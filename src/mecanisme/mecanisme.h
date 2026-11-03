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


bool VerreRamasse = false;

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
 * @brief Ramasse le verre.
 */
void RamasserVerre();

/**
 * @brief Dépose le verre.
 */
void DeposerVerre();

/**
 * @brief Effectue le mouvement nécessaire pour faire tomber une quille.
 */
void FaireTomberQuille();

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

#endif