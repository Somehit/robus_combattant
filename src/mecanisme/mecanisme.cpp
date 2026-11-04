#include "mecanisme/mecanisme.h"

bool VerreRamasse1 = false;

void MecanismeInit() //*******ajouter une position de base au pince??
{
    SERVO_Enable(0); // J6
    SERVO_SetAngle(0, 90); // Position initiale du servomoteur 0 (pince)

    delay(1000); //delay juste pour sassurer qu la pince est ouverte
}

void OuvrirPince()
{
    for(int angle = 55; angle <= 90; angle++)
    {
        SERVO_SetAngle(0, angle);
        //SERVO_SetAngle(1, 180 - angle);

        delay(10);
    }
}

void FermerPince()
{
    for(int angle = 90; angle >= 55; angle--)
    {
        SERVO_SetAngle(0, angle);
        //SERVO_SetAngle(1, 180 - angle);

        delay(10);
    }
}


void RamasserVerre()
{
    // Ouvre la pince
    OuvrirPince();

    delay(500);

    // Le robot devrait être positionné devant le verre

    // Referme la pince sur le verre
    FermerPince();

    VerreRamasse1 = true;

    delay(500);
}


void FaireTomberQuille()
{
    // Rien pour l'instant
    // La quille sera probablement renversée par le déplacement du robot.
}


void DeposerVerre()
{
    if(VerreRamasse1)
    {
        OuvrirPince();

        delay(500);

        VerreRamasse1 = false;
    }
}


bool VerreRamasse(){
    return VerreRamasse1;
}


/* RamasserVerre();

    while(true)
    {

    }
*/

/*void loop()
{
    FaireTomberQuille();

    while(true)
    {

    }
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
