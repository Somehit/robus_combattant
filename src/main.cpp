#include "config.h"

#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrarouge/infrarouge.h"
#include "mecanisme/mecanisme.h"
#include "obstacles/obstacles.h"
#include "suivi_ligne/suivi_ligne.h"

void setup() 
{
    Serial.begin(115200);

    BoardInit();
    MotorsInit();
    SUIVI_LIGNE_Initialisation();

}

void loop()
 {
SUIVI_LIGNE_Execution();
action_obstacle();

}


/*#include "config.h"

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