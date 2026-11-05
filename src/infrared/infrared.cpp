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

bool signalReceived() {
    static int clickCount = 0;
    static unsigned long timer = 0;

    static bool lastGreen = HIGH;
    static bool lastRed = HIGH;

    bool green = digitalRead(GREEN_IR_PIN);
    bool red = digitalRead(RED_IR_PIN);

    // Detect a new click (HIGH -> LOW)
    bool greenClick = (lastGreen == HIGH && green == LOW);
    bool redClick = (lastRed == HIGH && red == LOW);

    lastGreen = green;
    lastRed = red;

    // Either sensor can trigger a click
    if (greenClick || redClick) {
        if (clickCount == 0) {
            timer = millis();
        }

        clickCount++;

        if (clickCount >= 2 && millis() - timer < 3000) {
            clickCount = 0;
            return true;
        }
    }

    // Reset after 3 seconds
    if (clickCount > 0 && millis() - timer >= 3000) {
        clickCount = 0;
    }

    return false;
}