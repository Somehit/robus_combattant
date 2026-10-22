#include <config.h>
#include <motor/motor.h>
#include <infrared/infrared.h>
#include <pathfinding/navigation/navigation.h>
#include <wistle/wistle.h>

bool paused;

void setup() {
    Wire.begin();
    Wire.setWireTimeout(500, true);

    BoardInit();
    MotorsInit();
    InfraredInit();
    NavigationInit();
    WistleInit();
    paused = true;
}

void loop() {

}
