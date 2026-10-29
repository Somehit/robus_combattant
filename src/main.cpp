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


}


