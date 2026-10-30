
#include "config.h"
#include "couleur_del/couleur_del.h"

#include "Couleur.h"
#include "Mur/Mur.h"
#include "Arret/Arret.h"

bool jauneExecute = false;
bool bleuExecute = false;

void setup()
{
    Serial.begin(115200);

    BoardInit();

    if (!InitialiserCouleurDel()) {
        Serial.println("Erreur : capteur non detecte!");
    }
}

void loop() {
    ActualiserCouleurDel();
}
