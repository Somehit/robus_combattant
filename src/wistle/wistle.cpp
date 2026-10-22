#include <wistle/wistle.h>

void WistleInit() {
    pinMode(WISTLE_PIN, OUTPUT);
}

bool isWistleBlown() {
    return (analogRead(ANALOG_INPUT_1) - analogRead(ANALOG_INPUT_2)) > THRESHOLD;
}