#include "couleur_del.h"

Adafruit_TCS34725 capteur(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X
);

unsigned long debutDEL[5] = {0};
bool capteurPret = false;

bool CouleurDelInit() {
    for (int i = 0; i < 5; i++) {
        pinMode(BROCHES_LEDS[i], OUTPUT);
        digitalWrite(BROCHES_LEDS[i], LOW);
    }

    Wire.begin();
    capteurPret = capteur.begin();

    if (capteurPret) {
        Serial.println("Capteur couleur TCS34725: OK");
    } else {
        Serial.println("ERREUR: capteur couleur TCS34725 introuvable (verifier SDA/SCL/alimentation)");
    }

    return capteurPret;
}

void AllumerDel(CouleurDetectee couleur) {
    if (couleur == AUCUNE_COULEUR)
        return;

    int i = (int)couleur;

    // Allumer seulement si la DEL est eteinte
    if (digitalRead(BROCHES_LEDS[i]) == LOW) {
        digitalWrite(BROCHES_LEDS[i], HIGH);

        // Enregistrer le debut des 10 secondes
        debutDEL[i] = millis();
    }
}

void ActualiserDel() {
    for (int i = 0; i < 5; i++) {

        // Verifier si la DEL a depasse sa duree
        if (digitalRead(BROCHES_LEDS[i]) == HIGH &&
            millis() - debutDEL[i] >= DUREE_DEL) {

            digitalWrite(BROCHES_LEDS[i], LOW);
        }
    }
}

void EteindreDels() {
    for (int i = 0; i < 5; i++) {
        digitalWrite(BROCHES_LEDS[i], LOW);
    }
}

CouleurDetectee LireCouleur() {
    if (!capteurPret)
        return AUCUNE_COULEUR;

    float r, g, b;
    capteur.getRGB(&r, &g, &b);

    float total = r + g + b;

#if DEBUG_COULEUR
    Serial.print("RAW R="); Serial.print(r);
    Serial.print(" G="); Serial.print(g);
    Serial.print(" B="); Serial.print(b);
    Serial.print(" | ");
#endif

    if (total < 100) {
#if DEBUG_COULEUR
        Serial.println("total trop faible -> AUCUNE");
#endif
        return AUCUNE_COULEUR;
    }

    float rgb[3] = {
        (r / total) * 100,
        (g / total) * 100,
        (b / total) * 100
    };

    float meilleureDistance = 1e9;
    float deuxiemeDistance = 1e9;
    CouleurDetectee couleur = AUCUNE_COULEUR;

    for (int i = 0; i < 5; i++) {
        float distance = 0;

        for (int j = 0; j < 3; j++) {
            float difference = rgb[j] - centres[i][j];
            distance += difference * difference;
        }

        if (distance < meilleureDistance) {
            deuxiemeDistance = meilleureDistance;
            meilleureDistance = distance;
            couleur = (CouleurDetectee)i;
        } else if (distance < deuxiemeDistance) {
            deuxiemeDistance = distance;
        }
    }

#if DEBUG_COULEUR
    Serial.print("R% "); Serial.print(rgb[0]);
    Serial.print("  G% "); Serial.print(rgb[1]);
    Serial.print("  B% "); Serial.print(rgb[2]);
    Serial.print("  -> "); Serial.print((int)couleur);
    Serial.print("  dist "); Serial.println(meilleureDistance);
#endif

    // Rejeter les lectures trop differentes de la couleur calibree
    if (couleur != AUCUNE_COULEUR && meilleureDistance > DISTANCE_MAX[couleur]) {
        couleur = AUCUNE_COULEUR;
    }

    // Rejeter les lectures ambigues (presque aussi proches de 2 couleurs)
    if (couleur != AUCUNE_COULEUR && deuxiemeDistance < meilleureDistance * MARGE_AMBIGUITE) {
        couleur = AUCUNE_COULEUR;
    }

    // Allumer la DEL correspondante
    if (couleur != AUCUNE_COULEUR) {
        AllumerDel(couleur);
    }

    return couleur;
}
