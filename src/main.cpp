#include "config.h"
#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrarouge/infrarouge.h"
#include "mecanisme/mecanisme.h"
#include "obstacles/obstacles.h"
#include "suivi_ligne/suivi_ligne.h"

void setup() {
    Serial.begin(9600);
    BoardInit();
    MotorsInit();

    if (!InitialiserCouleurDel())
        Serial.println("Erreur : capteur non detecte.");
}

void loop() {
    ActualiserCouleurDel();

    // Les autres tâches du robot ici.
}
