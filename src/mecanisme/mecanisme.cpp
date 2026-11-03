#include "mecanisme/mecanisme.h"

// Indique si le robot transporte actuellement un verre
bool verreRamasse = false;

// Fonction permettant de savoir si un verre est transporté
bool VerreRamasse()
{
    return verreRamasse;
}

void MecanismeInit()
{
    SERVO_Enable(0); // J6
    SERVO_Enable(1); // J7

    delay(1000);
}

/**
 * @brief Ouvre progressivement la pince.
 */
void OuvrirPince()
{
    for(int angle = 90; angle >= 20; angle--)
    {
        SERVO_SetAngle(0, angle);
        SERVO_SetAngle(1, 180 - angle);

        delay(15);
    }
}

/**
 * @brief Ferme progressivement la pince.
 */
void FermerPince()
{
    for(int angle = 20; angle <= 90; angle++)
    {
        SERVO_SetAngle(0, angle);
        SERVO_SetAngle(1, 180 - angle);

        delay(15);
    }
}

/**
 * @brief Séquence de ramassage du verre.
 */
void RamasserVerre()
{
    // Ouvre la pince
    OuvrirPince();

    delay(500);

    // Le robot devrait être positionné devant le verre

    // Referme la pince sur le verre
    FermerPince();

    verreRamasse = true;

    delay(500);

}

/**
 * @brief Dépose le verre à la fin du parcours.
 */
void DeposerVerre()
{
    if(verreRamasse)
    {
        OuvrirPince();

        delay(500);

        verreRamasse = false;
    }
}


/* RamasserVerre();

    while(true)
    {

    }
*/


/**
 * @brief Test continu du mouvement des servomoteurs.
 */
void ComprehensionServo()
{
    OuvrirPince();

    delay(2000);

    FermerPince();

    delay(2000);
}
