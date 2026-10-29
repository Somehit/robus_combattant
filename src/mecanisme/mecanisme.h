#ifndef MECANISME_H
#define MECANISME_H

#include "config.h"



/**
 * @brief Initialise les servomoteurs du mécanisme.
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
 * @brief Indique si un verre est actuellement transporté.
 *
 * @return true si un verre est présent dans la pince.
 * @return false sinon.
 */
bool VerreRamasse();

/**
 * @brief Sert à tester le mouvement des servomoteurs.
 */
void ComprehensionServo();

#endif