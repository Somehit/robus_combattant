#include "infrarouge/infrarouge.h"

IRsend irsend;
Bouton start = {BOUTON_START, LOW, LOW, 0};
Bouton wake = {BOUTON_WAKE, LOW, LOW, 0};

void envoyerCode(uint8_t code)
{
  digitalWrite(LED_VERTE, LOW); // LED verte allumée

  // Trame NEC 32 bits: adresse, ~adresse, commande, ~commande
  unsigned long trame = ((unsigned long)ADRESSE_IR << 24) | ((unsigned long)(uint8_t)~ADRESSE_IR << 16) | ((unsigned long)code << 8) | (uint8_t)~code;
  irsend.sendNEC(trame, 32);

  digitalWrite(LED_VERTE, HIGH); // LED verte éteinte
}

bool recevoirCode(uint8_t &code)
{
  // REMOTE_read() (LibRobus) retourne 0 si rien n'a ete recu
  uint32_t trame = REMOTE_read();

  // 0 = rien
  if (trame == 0)
  {
    return false;
  }

  // Afficher toute trame recue, peu importe sa provenance
  Serial.print("IR recu: 0x");
  Serial.println(trame, HEX);

  // 0xFFFFFFFF = code de repetition NEC (bouton tenu)
  if (trame == 0xFFFFFFFF)
  {
    return false;
  }

  // Meme ordre que envoyerCode(): adresse, ~adresse, commande, ~commande
  uint8_t adresse = (trame >> 24) & 0xFF;
  uint8_t adresseInv = (trame >> 16) & 0xFF;
  uint8_t commande = (trame >> 8) & 0xFF;
  uint8_t commandeInv = trame & 0xFF;

  // Rejeter les trames corrompues ou d'une autre telecommande
  if (adresse != ADRESSE_IR || adresseInv != (uint8_t)~adresse || commandeInv != (uint8_t)~commande)
  {
    Serial.println("  -> ignoree (adresse ou inverses invalides)");
    return false;
  }

  code = commande;
  return true;
}

void attendreCode(uint8_t codeAttendu)
{
  // Arreter le robot: le suivi de ligne vient de mettre les moteurs en marche
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);

  Serial.print("Attente du code IR 0x");
  Serial.println(codeAttendu, HEX);

  uint8_t code = 0;
  while (!(recevoirCode(code) && code == codeAttendu))
  {
  }

  Serial.print("Code IR recu: 0x");
  Serial.println(code, HEX);
}

bool appuiDetecte(Bouton &bouton)
{
  bool lecture = digitalRead(bouton.broche);

  if (lecture != bouton.lecturePrecedente)
  {
    bouton.dernierChangement = millis();
    bouton.lecturePrecedente = lecture;
  }

  if (millis() - bouton.dernierChangement >= TEMPS_ANTI_REBOND)
  {
    if (lecture != bouton.etatStable)
    {
      bouton.etatStable = lecture;

      if (bouton.etatStable == HIGH)
      {
        return true;
      }
    }
  }

  return false;
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
