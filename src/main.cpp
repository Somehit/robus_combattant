
#include "config.h"
#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "mecanisme/mecanisme.h"
#include "suivi_ligne/suivi_ligne.h"
#include "obstacles/obstacles.h"
#include "infrarouge/infrarouge.h"
#include "infrared/infrared.h"
#include <Adafruit_TCS34725.h>

bool started = false;

// Eviter de repeter les commandes
bool isRedFinished = false;
bool isBlueFinished = false;
bool isGreenFinished = false;
bool isYellowFinished = false;
bool isOrangeFinished = false;

void setup()
{
    Serial.begin(9600);

    BoardInit();
    // Pas de setupInfrarouge() sur le robot: ses broches (4 et 7) sont celles
    // des servos 0 et 1. Elle sert seulement au code de la telecommande.
    CouleurDelInit();
    InfraredInit();
    MotorsInit();
    MecanismeInit();
    SuiviLigneInit();
}

void loop()
{
    // Actualiser les LEDs en tout temps
    ActualiserDel();

    if ((ROBUS_IsBumper(3) || (signalReceived())) && !started)
    {
        started = true;
        
        AX_BuzzerON();
        delay(1000);
        AX_BuzzerOFF();
        delay(500);
        AX_BuzzerON();
        delay(1000);
        AX_BuzzerOFF();
    }

    if (!started)
    {
        return;
    }

    // Detection de couleur
    CouleurDetectee couleur = LireCouleur();

    // Suivi de ligne
    SUIVI_LIGNE_Execution(0.45);

    // Detection du mur (jaune)
    if (couleur == JAUNE && !isYellowFinished)
    {
        contournerMur();
        isYellowFinished = true;
        return;
    }

    // Detection de la zone d'attente (bleu)
    if (couleur == BLEU && !isBlueFinished)
    {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);

        while (!signalReceived())
        {
        }

        AX_BuzzerON();
        delay(1000);
        AX_BuzzerOFF();
        delay(500);
        AX_BuzzerON();
        delay(1000);
        AX_BuzzerOFF();
        delay(500);
        AX_BuzzerON();
        delay(1000);
        AX_BuzzerOFF();

        isBlueFinished = true;
        return;
    }

    // Ramasser le verre (orange)
    if (couleur == ORANGE && !isOrangeFinished)
    {
        isOrangeFinished = true;
        RecupererVerre();
        return;
    }

    // Deposer le verre a la fin du parcours
    // TODO: remplacer finDuParcours par la vraie condition de fin
    // if (VerreRamasse() && finDuParcours)
    // {
    //     stopMotors();
    //     DeposerVerre();
    // }

    // Defoncer la quille (rouge)
    if (couleur == ROUGE && !isRedFinished)
    {
        isRedFinished = true;
        FaireTomberQuille();
        return;
    }

    // Passage dans le vide (vert)
    if (couleur == VERT && !isGreenFinished)
    {
        tout_droit();
        isGreenFinished = true;
        return;
    }
    if(isRedFinished && isOrangeFinished && isGreenFinished && isBlueFinished && isYellowFinished){
        stopMotors();
        OuvrirPince();
    }
}