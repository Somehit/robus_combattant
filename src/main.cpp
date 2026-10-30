
#include "config.h"
#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrarouge/infrarouge.h"
#include "mecanisme/mecanisme.h"
#include "obstacles/obstacles.h"
#include "suivi_ligne/suivi_ligne.h"

#include "Couleur.h"
#include "Mur.h"
#include "Arret.h"

bool jauneExecute = false;
bool bleuExecute = false;

void setup()
{
    Serial.begin(115200);

    BoardInit();

    MotorsInit();
    MecanismeInit();
    SUIVI_LIGNE_Initialisation();

    // Initialisation du capteur de couleur
    initCouleur();
}

void loop()
{
    Couleur couleur = detecterCouleur();

    if (couleur != JAUNE)
    {
        jauneExecute = false;
    }

    if (couleur != BLEU)
    {
        bleuExecute = false;
    }

    // Detection du mur (jaune)
    if (couleur == JAUNE && !jauneExecute)
    {
        jauneExecute = true;
        mur();
        return;
    }

    // Detection de la zone d'attente (bleu)
    if (couleur == BLEU && !bleuExecute)
    {
        bleuExecute = true;
        arret();
        return;
    }

    // Ajouter ici la fonction de suivi de ligne
}
