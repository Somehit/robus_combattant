#include <LibRobus.h>
#include "mecanisme/mecanisme.h"

void MecanismeInit()
{
    SERVO_Enable(0); // J6
    SERVO_Enable(1); // J7

    delay(1000);
}

void ComprehensionServo()
{
    // 0° -> 180°
    for(int angle = 0; angle <= 180; angle += 5)
    {
        SERVO_SetAngle(0, angle);         // J6
        SERVO_SetAngle(1, 180 - angle);   // J7 (miroir)

        delay(20);
    }

    // 180° -> 0°
    for(int angle = 180; angle >= 0; angle -= 5)
    {
        SERVO_SetAngle(0, angle);
        SERVO_SetAngle(1, 180 - angle);

        delay(20);
    }
}
