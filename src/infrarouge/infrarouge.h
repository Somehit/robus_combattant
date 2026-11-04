#ifndef INFRAROUGE_H
#define INFRAROUGE_H

#include <LibRobus.h> // inclut deja IRremote (ancienne API: IRsend)

extern IRsend irsend; // sur Arduino Mega, la LED IR est forcement sur la broche 9 (timer 2)

// Broches
const int BOUTON_START = 4;
const int BOUTON_WAKE = 5;

const int LED_ROUGE = 6;
const int LED_VERTE = 7;

const int LED_IR = 9;

// Codes infrarouges
const uint8_t ADRESSE_IR = 0x00;
const uint8_t CODE_START = 0x10;
const uint8_t CODE_WAKE = 0x20;

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
bool appuiDetecte(Bouton &bouton);
void setupInfrarouge();

#endif
