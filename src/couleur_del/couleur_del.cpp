#include "couleur_del.h"
#include <Wire.h>
#include <Adafruit_TCS34725.h>

namespace {
    constexpr uint8_t DELS[] = {9, 10, 11, 12, 13};
    constexpr unsigned long DUREE = 10000;
    constexpr unsigned long INTERVALLE = 100;
    constexpr uint8_t CONFIRMATIONS = 3;

    Adafruit_TCS34725 capteur(
        TCS34725_INTEGRATIONTIME_50MS,
        TCS34725_GAIN_4X
    );

    // Ordre : Rmin, Rmax, Gmin, Gmax,
    //         Bmin, Bmax, ClairMin, ClairMax
    struct Plage {
        uint16_t min[4], max[4];
    };

    // Remplacer les valeurs par celles des cartons.
    const Plage seuils[] = {
        {{0, 0, 0, 80}, {0, 0, 0, 65535}}, // Rouge
        {{0, 0, 0, 80}, {0, 0, 0, 65535}}, // Jaune
        {{0, 0, 0, 80}, {0, 0, 0, 65535}}, // Orange
        {{0, 0, 0, 80}, {0, 0, 0, 65535}}, // Bleu
        {{0, 0, 0, 80}, {0, 0, 0, 65535}}  // Vert
    };

    bool pret = false, delAllumee = false;
    unsigned long debut = 0, derniereLecture = 0;

    CouleurDetectee couleur = AUCUNE_COULEUR;
    CouleurDetectee candidate = AUCUNE_COULEUR;
    CouleurDetectee precedente = AUCUNE_COULEUR;
    uint8_t confirmations = 0;

    void eteindre() {
        for (uint8_t pin : DELS) digitalWrite(pin, LOW);
    }

    void allumer(CouleurDetectee c) {
        eteindre();
        digitalWrite(DELS[c - ROUGE], HIGH);
        debut = millis();
        delAllumee = true;
    }

    CouleurDetectee classifier(uint16_t r, uint16_t g,
                               uint16_t b, uint16_t clair) {
        uint16_t valeurs[] = {r, g, b, clair};
        int trouve = -1;

        for (uint8_t i = 0; i < 5; i++) {
            bool correspond = true;

            for (uint8_t j = 0; j < 4; j++) {
                if (valeurs[j] < seuils[i].min[j] ||
                    valeurs[j] > seuils[i].max[j]) {
                    correspond = false;
                    break;
                }
            }

            if (correspond) {
                if (trouve != -1) return AUCUNE_COULEUR;
                trouve = i;
            }
        }

        return trouve == -1
            ? AUCUNE_COULEUR
            : static_cast<CouleurDetectee>(ROUGE + trouve);
    }
}

bool InitialiserCouleurDel() {
    for (uint8_t pin : DELS) pinMode(pin, OUTPUT);
    eteindre();
    Wire.begin();
    pret = capteur.begin();
    return pret;
}

void ActualiserCouleurDel() {
    if (!pret) return;

    unsigned long maintenant = millis();

    if (delAllumee && maintenant - debut >= DUREE) {
        eteindre();
        delAllumee = false;
    }

    if (maintenant - derniereLecture < INTERVALLE) return;
    derniereLecture = maintenant;

    uint16_t r, g, b, clair;
    capteur.getRawData(&r, &g, &b, &clair);

    CouleurDetectee nouvelle = classifier(r, g, b, clair);

    if (nouvelle != candidate) {
        candidate = nouvelle;
        confirmations = 1;
    } else if (confirmations < CONFIRMATIONS) {
        confirmations++;
    }

    if (confirmations < CONFIRMATIONS) return;

    couleur = candidate;

    if (couleur != AUCUNE_COULEUR && couleur != precedente) {
        allumer(couleur);
        Serial.print("Couleur : ");
        Serial.println(static_cast<int>(couleur));
    }

    precedente = couleur;
}

CouleurDetectee ObtenirCouleurDetectee() {
    return couleur;
}
