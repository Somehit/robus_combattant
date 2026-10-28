#include "couleur_del.h"

#include <Wire.h>
#include <Adafruit_TCS34725.h>

namespace {

    constexpr uint8_t DEL_ROUGE  = 9;
    constexpr uint8_t DEL_JAUNE  = 10;
    constexpr uint8_t DEL_ORANGE = 11;
    constexpr uint8_t DEL_BLEUE  = 12;
    constexpr uint8_t DEL_VERTE  = 13;

    constexpr unsigned long DUREE_DEL = 10000;
    constexpr unsigned long INTERVALLE_LECTURE = 100;

    constexpr uint8_t LECTURES_REQUISES = 3;

    Adafruit_TCS34725 capteur(
        TCS34725_INTEGRATIONTIME_50MS,
        TCS34725_GAIN_4X
    );

    bool capteurPret = false;

    const uint8_t BROCHES_DEL[] = {
        DEL_ROUGE, DEL_JAUNE, DEL_ORANGE, DEL_BLEUE, DEL_VERTE
    };

    struct SeuilCouleur {
        uint16_t rMin, rMax;
        uint16_t gMin, gMax;
        uint16_t bMin, bMax;
        uint16_t clairMin, clairMax;
    };

    const SeuilCouleur SEUILS_ROUGE = {
        0, 0, 0, 0, 0, 0, 80, 65535
    };

    const SeuilCouleur SEUILS_JAUNE = {
        0, 0, 0, 0, 0, 0, 80, 65535
    };

    const SeuilCouleur SEUILS_ORANGE = {
        0, 0, 0, 0, 0, 0, 80, 65535
    };

    const SeuilCouleur SEUILS_BLEU = {
        0, 0, 0, 0, 0, 0, 80, 65535
    };

    const SeuilCouleur SEUILS_VERT = {
        0, 0, 0, 0, 0, 0, 80, 65535
    };

    CouleurDetectee couleurDetectee = AUCUNE_COULEUR;
    CouleurDetectee couleurCandidate = AUCUNE_COULEUR;
    CouleurDetectee derniereCouleurValidee = AUCUNE_COULEUR;

    uint8_t lecturesConsecutives = 0;

    unsigned long debutDel = 0;
    unsigned long derniereLecture = 0;

    bool delAllumee = false;


    void eteindreToutesLesDel() {
        for (uint8_t broche : BROCHES_DEL) {
            digitalWrite(broche, LOW);
        }
    }

    void allumerDel(CouleurDetectee couleur) {
        eteindreToutesLesDel();

        switch (couleur) {
            case ROUGE:
                digitalWrite(DEL_ROUGE, HIGH);
                break;

            case JAUNE:
                digitalWrite(DEL_JAUNE, HIGH);
                break;

            case ORANGE:
                digitalWrite(DEL_ORANGE, HIGH);
                break;

            case BLEU:
                digitalWrite(DEL_BLEUE, HIGH);
                break;

            case VERT:
                digitalWrite(DEL_VERTE, HIGH);
                break;

            default:
                return;
        }

        debutDel = millis();
        delAllumee = true;
    }

    bool correspondAuxSeuils(
        uint16_t r,
        uint16_t g,
        uint16_t b,
        uint16_t clair,
        const SeuilCouleur& seuils
    ) {
        return
            r >= seuils.rMin && r <= seuils.rMax &&
            g >= seuils.gMin && g <= seuils.gMax &&
            b >= seuils.bMin && b <= seuils.bMax &&
            clair >= seuils.clairMin &&
            clair <= seuils.clairMax;
    }

    CouleurDetectee classifierCouleur(
        uint16_t r,
        uint16_t g,
        uint16_t b,
        uint16_t clair
    ) {
        CouleurDetectee resultat = AUCUNE_COULEUR;
        uint8_t nombreCorrespondances = 0;

        if (correspondAuxSeuils(r, g, b, clair, SEUILS_ROUGE)) {
            resultat = ROUGE;
            nombreCorrespondances++;
        }

        if (correspondAuxSeuils(r, g, b, clair, SEUILS_JAUNE)) {
            resultat = JAUNE;
            nombreCorrespondances++;
        }

        if (correspondAuxSeuils(r, g, b, clair, SEUILS_ORANGE)) {
            resultat = ORANGE;
            nombreCorrespondances++;
        }

        if (correspondAuxSeuils(r, g, b, clair, SEUILS_BLEU)) {
            resultat = BLEU;
            nombreCorrespondances++;
        }

        if (correspondAuxSeuils(r, g, b, clair, SEUILS_VERT)) {
            resultat = VERT;
            nombreCorrespondances++;
        }

        if (nombreCorrespondances != 1) {
            return AUCUNE_COULEUR;
        }

        return resultat;
    }
}

bool InitialiserCouleurDel() {
    for (uint8_t broche : BROCHES_DEL) {
        pinMode(broche, OUTPUT);
    }

    eteindreToutesLesDel();

    Wire.begin();
    capteurPret = capteur.begin();

    if (capteurPret) {
        Serial.println("Capteur de couleur initialise.");
    } else {
        Serial.println("ERREUR : capteur non detecte.");
    }

    return capteurPret;
}

void ActualiserCouleurDel() {
    if (!capteurPret) {
        return;
    }

    unsigned long maintenant = millis();

    if (delAllumee && maintenant - debutDel >= DUREE_DEL) {
        eteindreToutesLesDel();
        delAllumee = false;
    }

    if (maintenant - derniereLecture < INTERVALLE_LECTURE) {
        return;
    }

    derniereLecture = maintenant;

    uint16_t r, g, b, clair;
    capteur.getRawData(&r, &g, &b, &clair);

    CouleurDetectee nouvelleCouleur =
        classifierCouleur(r, g, b, clair);

    // Exiger plusieurs lectures identiques.
    if (nouvelleCouleur == couleurCandidate) {
        if (lecturesConsecutives < LECTURES_REQUISES) {
            lecturesConsecutives++;
        }
    } else {
        couleurCandidate = nouvelleCouleur;
        lecturesConsecutives = 1;
    }

    if (lecturesConsecutives < LECTURES_REQUISES) {
        return;
    }

    couleurDetectee = couleurCandidate;

    if (couleurDetectee != AUCUNE_COULEUR &&
        couleurDetectee != derniereCouleurValidee) {

        allumerDel(couleurDetectee);

        Serial.print("Couleur detectee : ");
        Serial.println(static_cast<int>(couleurDetectee));
    }

    derniereCouleurValidee = couleurDetectee;
}

CouleurDetectee ObtenirCouleurDetectee() {
    return couleurDetectee;
}
