
#include "couleur_del.h"
#include <Adafruit_TCS34725.h>

Adafruit_TCS34725 capteur(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X
);

CouleurDetectee couleur = AUCUNE_COULEUR;

unsigned long derniereLecture = 0;
unsigned long debutDEL = 0;

bool pret = false;
bool delActive = false;

bool InitialiserCouleurDel() {
    for (int i = 0; i < 5; i++) {
        pinMode(BROCHES_LEDS[i], OUTPUT);
        digitalWrite(BROCHES_LEDS[i], LOW);
    }

    Wire.begin();
    pret = capteur.begin();

    return pret;
}

void ActualiserCouleurDel() {
    if (!pret)
        return;

    // Lecture du capteur
    if (millis() - derniereLecture >= INTERVALLE_LECTURE) {
        derniereLecture = millis();

        float r, g, b;
        capteur.getRGB(&r, &g, &b);

        Serial.print("R: ");
        Serial.print(r);
        Serial.print(" | G: ");
        Serial.print(g);
        Serial.print(" | B: ");
        Serial.println(b);

        float valeurs[] = {r, g, b};
        CouleurDetectee nouvelle = AUCUNE_COULEUR;

        // Comparaison avec les seuils
        for (int i = 0; i < 5; i++) {
            bool correspond = true;

            for (int j = 0; j < 3; j++) {
                if (valeurs[j] < SEUILS_MIN[i][j] ||
                    valeurs[j] > SEUILS_MAX[i][j]) {
                    correspond = false;
                    break;
                }
            }

            if (correspond) {
                if (nouvelle != AUCUNE_COULEUR) {
                    nouvelle = AUCUNE_COULEUR;
                    break;
                }

                nouvelle = (CouleurDetectee)i;
            }
        }

        Serial.print("Nouvelle couleur : ");
        Serial.println(nouvelle);

        // Nouvelle couleur détectée
        if (nouvelle != couleur) {
            couleur = nouvelle;

            if (couleur != AUCUNE_COULEUR && !delActive) {
                digitalWrite(BROCHES_LEDS[couleur], HIGH);

                debutDEL = millis();
                delActive = true;

                Serial.print("Couleur detectee : ");
                Serial.println(couleur);
            }
        }
    }

    // Extinction après 10 secondes
    if (delActive && millis() - debutDEL >= DUREE_DEL) {
        for (int i = 0; i < 5; i++) {
            digitalWrite(BROCHES_LEDS[i], LOW);
        }

        delActive = false;
    }
}

CouleurDetectee ObtenirCouleurDetectee() {
    return couleur;
}
