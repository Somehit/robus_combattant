#ifndef SUIVI_LIGNE
#define SUIVI_LIGNE

#include "config.h"

// --- CONSTANTES DU MODULE ---
const uint8_t PIN_CAPTEUR_GAUCHE = A2; //U3
const uint8_t PIN_CAPTEUR_CENTRE = A1; //U2
const uint8_t PIN_CAPTEUR_DROITE = A0; //U1
const int SEUIL_NOIR = 500;
const int SEUIL_MINOIR = 250;
void SUIVI_LIGNE_Initialisation();
void SUIVI_LIGNE_Execution();


#endif