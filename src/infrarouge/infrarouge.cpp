#include <infrarouge/infrarouge.h>

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