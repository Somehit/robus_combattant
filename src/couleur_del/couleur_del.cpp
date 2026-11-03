#include "couleur_del.h"
#include <Adafruit_TCS34725.h>

static Adafruit_TCS34725 capteur(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X
);

static bool capteurPret = false;
static bool delActive = false;
static unsigned long debutDEL = 0;

bool InitialiserCouleurDel() {
    for (int i = 0; i < 5; i++) {
        pinMode(BROCHES_LEDS[i], OUTPUT);
        digitalWrite(BROCHES_LEDS[i], LOW);
    }

    Wire.begin();
    capteurPret = capteur.begin();
    return capteurPret;
}

CouleurDetectee LireCouleur() {
    if (!capteurPret)
        return AUCUNE_COULEUR;

    float r, g, b;
    capteur.getRGB(&r, &g, &b);

    CouleurDetectee trouvee = AUCUNE_COULEUR;
    int nbCorrespondances = 0;

    for (int i = 0; i < 5; i++) {
        if (r >= SEUILS_MIN[i][0] && r <= SEUILS_MAX[i][0] &&
            g >= SEUILS_MIN[i][1] && g <= SEUILS_MAX[i][1] &&
            b >= SEUILS_MIN[i][2] && b <= SEUILS_MAX[i][2]) {
            trouvee = (CouleurDetectee)i;
            nbCorrespondances++;
        }
    }

    // ambiguïté si plus qu'une couleur marche
    if (nbCorrespondances != 1)
        return AUCUNE_COULEUR;

    return trouvee;
}

void AllumerDel(CouleurDetectee couleur) {
    if (couleur == AUCUNE_COULEUR || delActive)
        return;

    digitalWrite(BROCHES_LEDS[couleur], HIGH);
    debutDEL = millis();
    delActive = true;
}

void ActualiserDel() {
    if (delActive && millis() - debutDEL >= DUREE_DEL) {
        EteindreDels();
    }
}

void EteindreDels() {
    for (int i = 0; i < 5; i++) {
        digitalWrite(BROCHES_LEDS[i], LOW);
    }
    delActive = false;
}
