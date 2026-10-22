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

    // Reagir seulement au moment ou le bumper est enfonce (pas tant qu'il reste enfonce)

    if (isWistleBlown() || ROBUS_IsBumper(3)) {
        if (paused) {
            NavigationInit();
            paused = false;
        } else {
            paused = true;
        }
    }

    if (paused) {
        return;
    }

    if (hasReachedEnd()) {
        reverseAllMovesInHistory();
        paused = true;
        return;
    }

    moveFromPosRobot(getPosition().column);
}
