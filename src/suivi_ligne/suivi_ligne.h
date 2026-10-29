#ifndef SUIVI_LIGNE
#define SUIVI_LIGNE

#include "config.h"

// --- CONSTANTES DU MODULE ---
constexpr uint8_t PIN_CAPTEUR_GAUCHE = A2; //U3
constexpr uint8_t PIN_CAPTEUR_CENTRE = A1; //U2
constexpr uint8_t PIN_CAPTEUR_DROITE = A0; //U1
constexpr int SEUIL_C =175;// seuil depend vraiment de la reflexion de la ligne, selon eclairage et tape
constexpr int SEUIL_G =273;//chacun des capteur en a des different (valeur noire+ valeur blanc/2) 
constexpr int SEUIL_D =114;//ici selon jeudi, trois ligne diff donnait des truc diff alors jai fait leur moyenne

/**
 * @brief initialiser suiveur de ligne
 */
void SUIVI_LIGNE_Initialisation();

/**
 *  @brief suivre la ligne
 */
void SUIVI_LIGNE_Execution(float facteur= 0.7);


#endif
/*testage capteur ligne (copier sa dans le main en metant le reste en commentaire pour tester seuil)
#include "config.h"

#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrarouge/infrarouge.h"
#include "mecanisme/mecanisme.h"
#include "obstacles/obstacles.h"
#include "suivi_ligne/suivi_ligne.h"

void setup() {
    Serial.begin(9600);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
}

void loop() {
    int g = analogRead(A0);
    int c = analogRead(A1);
    int d = analogRead(A2);

    Serial.print("Gauche: ");
    Serial.print(g);
    Serial.print("   Centre: ");
    Serial.print(c);
    Serial.print("   Droite: ");
    Serial.println(d);

    delay(200);
}*/