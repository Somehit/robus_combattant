#ifndef INFRAROUGE_H
#define INFRAROUGE_H

#include <LibRobus.h> // inclut deja IRremote (ancienne API: IRsend)
#include "motor/motor.h"

extern IRsend irsend; // sur Arduino Mega, la LED IR est forcement sur la broche 9 (timer 2)

// Broches
const int BOUTON_START = 4;
const int BOUTON_WAKE = 5;

const int LED_ROUGE = 6;
const int LED_VERTE = 7;

const int LED_IR = 9;

// Codes infrarouges
const uint8_t ADRESSE_IR = 0x10;
const uint8_t CODE_START = 0x20;
const uint8_t CODE_WAKE = 0x30;

// Gestion anti-rebond
const unsigned long TEMPS_ANTI_REBOND = 50;

struct Bouton
{
    int broche;
    bool lecturePrecedente;
    bool etatStable;
    unsigned long dernierChangement;
};

extern Bouton start;
extern Bouton wake;

void envoyerCode(uint8_t code);

/**
 * @brief Lit le recepteur IR de LibRobus (broche 37) sans bloquer.
 *        Verifie la trame NEC (adresse + inverses) pour rejeter le bruit.
 * @param code Recoit la commande si une trame valide est arrivee
 * @return true si une trame valide pour ADRESSE_IR a ete recue
 */
bool recevoirCode(uint8_t &code);

/**
 * @brief Arrete le robot et attend de recevoir un code IR precis.
 */
void attendreCode(uint8_t codeAttendu);

bool appuiDetecte(Bouton &bouton);

void setupInfrarouge();

#endif
