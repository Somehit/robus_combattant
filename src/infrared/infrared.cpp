#include <infrared/infrared.h>// dossier ajouter pour tester les fonction obstacle

void InfraredInit() {
    pinMode(GREEN_IR_PIN, INPUT);
    pinMode(RED_IR_PIN, INPUT);
}

bool isObstacle() {
    bool green = digitalRead(GREEN_IR_PIN);
    bool red = digitalRead(RED_IR_PIN);

    return !(green == HIGH || red == HIGH);
}