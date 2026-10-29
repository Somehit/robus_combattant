#ifndef SUIVI_LIGNE
#define SUIVI_LIGNE

#include "config.h"

// --- CONSTANTES DU MODULE ---
constexpr uint8_t PIN_CAPTEUR_GAUCHE = A2; //U3
constexpr uint8_t PIN_CAPTEUR_CENTRE = A1; //U2
constexpr uint8_t PIN_CAPTEUR_DROITE = A0; //U1
constexpr int SEUIL_NOIR = 500;
constexpr int SEUIL_MINOIR = 250;

/**
 * @brief initialiser suiveur de ligne
 */
void SUIVI_LIGNE_Initialisation();

/**
 *  @brief suivre la ligne
 */
void SUIVI_LIGNE_Execution();


#endif