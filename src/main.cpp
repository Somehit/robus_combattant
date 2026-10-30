#include "config.h"

#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "mecanisme/mecanisme.h"
#include "suivi_ligne/suivi_ligne.h"

#include "Mur/Mur.h"
#include "Arret/Arret.h"
#include "obstacles/obstacles.h"
bool roseExecute = false;
bool jauneExecute = false;
bool orangeExecute = false;
bool vertExecute = false;

void setup()
{
    Serial.begin(115200);

    BoardInit();

    MotorsInit();
    MecanismeInit();
    SUIVI_LIGNE_Initialisation();
    InfraredInit();

    if (!InitialiserCouleurDel())
    {
        Serial.println("Erreur : capteur non detecte!");
    }
}

void loop()
{
    SUIVI_LIGNE_Execution();

    ActualiserCouleurDel();

    CouleurDetectee couleur = ObtenirCouleurDetectee();

    switch(couleur)
    {
        case ROUGE:

            if(!roseExecute)
            {
                Serial.println("Mission ROSE");

                FaireTomberQuille();

                roseExecute = true;
            }
            break;

        case JAUNE:

            if(!jauneExecute)
            {
                Serial.println("Mission JAUNE");

                mur();

                jauneExecute = true;
            }
            break;

        case ORANGE:

            if(!orangeExecute)
            {
                Serial.println("Mission ORANGE");

                RamasserVerre();

                orangeExecute = true;
            }
            break;

        case VERT:

            if(!vertExecute)
            {
                Serial.println("Mission VERTE");

                tout_droit();

                vertExecute = true;
            }
            break;

        default:
            break;
    }

    // Fin du parcours
    if(roseExecute &&
       jauneExecute &&
       orangeExecute &&
       vertExecute)
    {
        if(VerreRamasse())
        {
            DeposerVerre();
        }

        while(true)
        {
        }
    }
}