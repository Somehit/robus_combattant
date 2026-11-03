//#include "infrarouge/infrarouge.h"

// Voir la librairie suivante pour les fonctions disponible: https://github.com/crankyoldgit/IRremoteESP8266
// Ne pas re-importer, ca deja etait fait dans le .h
#include <LibRobus.h> // inclut deja IRremote (ancienne API: IRsend)

IRsend irsend; // sur Arduino Mega, la LED IR est forcement sur la broche 9 (timer 2)

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

struct Bouton {
int broche;
bool lecturePrecedente;
bool etatStable;
unsigned long dernierChangement;
};

Bouton start = {BOUTON_START, LOW, LOW, 0};
Bouton wake = {BOUTON_WAKE, LOW, LOW, 0};

void envoyerCode(uint8_t code) {
digitalWrite(LED_VERTE, LOW); // LED verte allumée

// Trame NEC 32 bits: adresse, ~adresse, commande, ~commande
unsigned long trame = ((unsigned long)ADRESSE_IR << 24) | ((unsigned long)(uint8_t)~ADRESSE_IR << 16) | ((unsigned long)code << 8) | (uint8_t)~code;
irsend.sendNEC(trame, 32);

digitalWrite(LED_VERTE, HIGH); // LED verte éteinte
}

bool appuiDetecte(Bouton &bouton) {
bool lecture = digitalRead(bouton.broche);

if (lecture != bouton.lecturePrecedente) {
bouton.dernierChangement = millis();
bouton.lecturePrecedente = lecture;
}

if (millis() - bouton.dernierChangement >= TEMPS_ANTI_REBOND) {
if (lecture != bouton.etatStable) {
bouton.etatStable = lecture;

if (bouton.etatStable == HIGH) {
return true;
}
}
}

return false;
}

void setupInfrarouge() {
pinMode(BOUTON_START, INPUT);
pinMode(BOUTON_WAKE, INPUT);

pinMode(LED_ROUGE, OUTPUT);
pinMode(LED_VERTE, OUTPUT);

// LED rouge allumée au démarrage
digitalWrite(LED_ROUGE, LOW);

// LED verte éteinte
digitalWrite(LED_VERTE, HIGH);

Serial.begin(9600);
Serial.println("Telecommande prete");
}

/*void loopInfrarouge() {

if (appuiDetecte(start)) {
Serial.println("Envoi START");
envoyerCode(CODE_START);
}

if (appuiDetecte(wake)) {
Serial.println("Envoi WAKE");
envoyerCode(CODE_WAKE);
}
}
*/