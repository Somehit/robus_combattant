#ifndef OBSTACLES
#define OBSTACLES

#include "config.h"

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
 * @brief Fait avancer le robot tout droit jusqu'à ce qu'il ne détecte plus d'obstacle.
 */
void tout_droit();

// Fin

#endif