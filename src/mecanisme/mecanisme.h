/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    mecanisme.h
 *  @brief   commande des servomoteurs
 *
 *  @details ouvrir/fermer la pince
 *           ramasser/déposer le verre
 *           Faire tomber la quille 
 *
 *  @author  Jérémy
 *  @date    2026-11-02
 *  @version 1.0
 * ============================================================================
 */

#ifndef MECANISME_H
#define MECANISME_H

#include "config.h"


constexpr int msParDegree = 5;

// --- QUILLE (rouge) ---
constexpr float QUILLE_DISTANCE_MAX_M = 0.5f;   // portee de recherche au sonar

// --- VERRE (orange) ---
constexpr float VERRE_DISTANCE_MAX_M = 0.5f;    // portee de recherche au sonar
constexpr float VERRE_DISTANCE_PRISE_M = 0.05f; // A MESURER: distance sonar->verre quand le verre est dans la pince


/**
 * @brief Initialise les servomoteurs du mécanisme et ouvre les pinces
 */
void MecanismeInit();

/**
 * @brief Ouvre progressivement la pince.
 */
void OuvrirPince();

/**
 * @brief Ferme progressivement la pince.
 */
void FermerPince();


/**
 * @brief Dépose le verre.
 */
void DeposerVerre();

/**
 * @brief Effectue le mouvement nécessaire pour faire tomber une quille.
 */
void FaireTomberQuille();

/**
 * @brief Trouve le verre au sonar, le prend avec la pince, revient sur la ligne.
 *        Le verre reste dans la pince (le deposer avec DeposerVerre()).
 */
void RecupererVerre();

// /**
//  * @brief Indique si un verre est actuellement transporté.
//  *
//  * @return true si un verre est présent dans la pince.
//  * @return false sinon.
//  */
// bool VerreRamasse();


/**
 * @brief Sert à tester le mouvement des servomoteurs.
 */
void ComprehensionServo();

bool VerreRamasse();

#endif