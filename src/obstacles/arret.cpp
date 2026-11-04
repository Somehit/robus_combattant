#include "obstacles/arret.h"

void arret()
{
    // Arrêter les deux moteurs
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);

    // Attendre une commande infrarouge
    while (REMOTE_read() == 0)
    {
        delay(10);
    }

    // Émettre trois bips
    for (int i = 0; i < 3; i++)
    {
        AX_BuzzerON(1000);
        delay(150);

        AX_BuzzerOFF();
        delay(150);
    }
}
