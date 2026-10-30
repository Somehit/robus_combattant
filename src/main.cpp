#include "config.h"
#include "couleur_del/couleur_del.h"

void setup() {
    Serial.begin(9600);
    BoardInit();

    if (!InitialiserCouleurDel()) {
        Serial.println("Erreur : capteur non detecte!");
    }
}

void loop() {
    ActualiserCouleurDel();
}
