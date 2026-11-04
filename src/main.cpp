/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    main.cpp
 *  @brief   logique global du code
 *  @details ce qu'il reste: compléter la logique du main
 *  @author  tlm 
 *  @date    2026-11-02
 *  @version 1.0
 * ============================================================================
 */

#include "config.h"
#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "mecanisme/mecanisme.h"
#include "suivi_ligne/suivi_ligne.h"
#include "obstacles/obstacles.h"
#include "infrarouge/infrarouge.h"

bool started = false;

// safeguard pour ne pas répéter les commandes d'un if
bool isRedFinished = false;
bool isBlueFinished = false;
bool isGreenFinished = false;
bool isYellowFinished = false;
bool isOrangeFinished = false;

void setup() // setup tous les .h 
{
    Serial.begin(9600); //meme valeur que dans monitor_speed dans le .ini
    BoardInit(); // LibRobus
    setupInfrarouge();
    CouleurDelInit(); //Pins 40, 41, 42, 43, 44 en output et set a LOW
    InfraredInit();   //Broches IR 48/49
    MotorsInit();     //encodeurs à 0, moteurs arretés, choix du robot
    MecanismeInit();  //Servomoteurs 0 et 1
    SuiviLigneInit(); //Pins A0, A1, A2 en entrée
}


void loop()
{
    //logique pour commencer
    if(ROBUS_IsBumper(3) || appuiDetecte(start)){
        if(started) 
            started = false;
        started = true;
    }
    //LOGIQUE une fois commencé
    if(started){
        //start();//---------
        SequenceDels();
        SUIVI_LIGNE_Execution(0.7);

        // Detection du mur (jaune)
        if (CouleurActuelle() == JAUNE && !isYellowFinished)
        {
            contournerMur();
            isYellowFinished = true;
            return;
        }

        // Detection de la zone d'attente (bleu)
        if (CouleurActuelle() == BLEU && !isBlueFinished)
        {
            attendreSignalDepart();//-------
            isBlueFinished = true;
            return;
        }

        // rammasser le verre
        if (CouleurActuelle() == ORANGE && !isOrangeFinished)
        {
            //trouver_verre();//------
            RamasserVerre();
            //retour_ligne();//-------
            isOrangeFinished = true;
            return;
        }

        // defoncer la quille
        if (CouleurActuelle() == ROUGE && !isRedFinished)
        {
            //trouver_quille();//-------
            FaireTomberQuille();//-----
            //retour_ligne();//---------
            isRedFinished = true;
            return;
        }

        // passe dans le vide
        if (CouleurActuelle() == VERT && !isGreenFinished)
        {
            tout_droit();
            isGreenFinished = true;
            return;
        }
    }
return;
    
}