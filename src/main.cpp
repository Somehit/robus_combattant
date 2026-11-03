
#include "config.h"

#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "mecanisme/mecanisme.h"
#include "suivi_ligne/suivi_ligne.h"
#include "obstacles/obstacles.h"

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
    InitialiserCouleurDel();
}

void loop()
{
    CouleurDetectee couleur = LireCouleur();

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
        contournerMur();
        return;
    }

    // Detection de la zone d'attente (bleu)
    if (couleur == BLEU && !bleuExecute)
    {
        bleuExecute = true;
        attendreSignalDepart();
        return;
    }

    // Ajouter ici la fonction de suivi de ligne
}
