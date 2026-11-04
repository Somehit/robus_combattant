/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    obstacles.h
 *  @brief   Franchsissmeent des obstacles du parcours
 *
 *  @details contourner un obstacle (devier_obstacle)
 *           et passer le trou tout droit en se réalignant sur la ligne (tout_droit).
 *
 *  @author  Naguy
 *  @date    2026-11-02
 *  @version 1.0
 * ============================================================================
 */

#ifndef OBSTACLES
#define OBSTACLES

#include "config.h"
#include "infrarouge/infrarouge.h"
#include "config.h"
#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrared/infrared.h"   // ou "infrared/infrared/infrared.h" selon ton dossier
#include "suivi_ligne/suivi_ligne.h"

/**
 * @brief choisit l'action selon couleur
 */
void action_obstacle();

/**
 * @brief anciennement dans mur.h, coutourne le mur (doit être appelée quand le robot voit du jaune)
 */
void contournerMur(); 

/**
 * @brief anciennement dans arret.h s'arrete et attend le signal IR (doit etre appelle quand il voit du bleu)
 */
void attendreSignalDepart();

// A FAIRE
void stopMotors();
//bool messageReveilRecul();



/**
 * @brief Fait avancer le robot tout droit jusqu'à ce qu'il ne détecte plus d'obstacle.
 */
void tout_droit();

// Fin

#endif