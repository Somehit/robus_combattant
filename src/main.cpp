#include "config.h"

#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrarouge/infrarouge.h"
#include "mecanisme/mecanisme.h"
#include "obstacles/obstacles.h"
#include "suivi_ligne/suivi_ligne.h"

void setup()
{
    Serial.begin(115200);

    BoardInit();

    MotorsInit();
    MecanismeInit();
    SUIVI_LIGNE_Initialisation();

    // Ajouter ici les autres fonctions d'initialisation
    // lorsqu'elles seront disponibles
}

void loop()
{
    
}

/* TEST

CouleurDetectee derniereMission = AUCUNE_COULEUR;


/*void loop()
{
    ActualiserCouleurDel();

    CouleurDetectee couleur = ObtenirCouleurDetectee();

    if (couleur != derniereMission)
    {
        switch(couleur)
        {
            case ORANGE:

                Serial.println("Ramassage du verre");

                RamasserVerre();

                break;

            case ROSE:

                Serial.println("Quille detectee");

                FaireTomberQuille();

                break;

            default:
                break;
        }

        derniereMission = couleur;
    }
}
*/