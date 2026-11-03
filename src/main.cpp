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

bool jauneExecute = false;
bool bleuExecute = false;

void setup() // setup tous les .h 
{
    Serial.begin(9600); //meme valeur que dans monitor_speed dans le .ini
    BoardInit(); // LibRobus

    CouleurDelInit(); //Pins 40, 41, 42, 43, 44 en output et set a LOW
    InfraredInit();   //Broches IR 48/49
    MotorsInit();     //encodeurs à 0, moteurs arretés, choix du robot
    MecanismeInit();  //Servomoteurs 0 et 1
    SuiviLigneInit(); //Pins A0, A1, A2 en entrée
    
}

void loop()
{
    delay(5000);
    SERVO_SetAngle(0, 55);

    delay(110500);
    float a;
    a = SONAR_GetRange(0);

   Serial.print("\n");
   Serial.print(a);
   Serial.println("cm");
   delay(5000);

   OuvrirPince();

   delay(110500);

    // Ajouter ici la fonction de suivi de ligne
}
