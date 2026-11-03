
#include "Arret.h"
#include <motor/motor.h>
#include <infrared/infrared.h>

void stopMotors();
bool messageReveilRecu();
void beep();

void arret() {

    stopMotors();

    
    while (!messageReveilRecu()) {
        delay(10);
    }

    
    for (int i = 0; i < 3; i++) {
        beep();
        delay(150);
    }
}
