#include "mecanisme/mecanisme.h"
#include "motor/motor.h"
#include "obstacles/obstacles.h"
#include "couleur_del/couleur_del.h"
#include "suivi_ligne/suivi_ligne.h"

bool VerreRamasse1;
bool PinceFerme;

void MecanismeInit()
{
    SERVO_Enable(0); // J6 (broche 4)
    SERVO_Disable(1); // servo 1 (broche 7) pas utilise: arreter ses impulsions
    SERVO_SetAngle(0, 110); // Position initiale du servomoteur 0 (pince)
    VerreRamasse1 = false;
    PinceFerme = false;


    delay(1000); //delay juste pour sassurer qu la pince est ouverte
}

void OuvrirPince() {
  if (PinceFerme == true) {
    for (int angle = 55; angle <= 110; angle++) {
      SERVO_SetAngle(0, angle);
      delay(msParDegree);
    }
    PinceFerme = false;
  }

  return;
}

void FermerPince() {
  if (PinceFerme == false) {
    for (int angle = 110; angle >= 0; angle--) {
      SERVO_SetAngle(0, angle);
      delay(msParDegree);
    }
    PinceFerme = true;
  }
  return;
}




// Tourne vers la droite jusqu'a trouver un objet (quille ou verre) au sonar.
// rotationCounts recoit la rotation effectuee, pour pouvoir revenir ensuite.
// Retourne la distance de l'objet en metres, ou -1 si rien trouve en un tour.
static float balayerVersObjet(float distanceMaxM, int32_t &rotationCounts)
{
    stopMotors();
    delay(100);

    ENCODER_ReadReset(LEFT);
    ENCODER_ReadReset(RIGHT);

    // Limite de balayage: un tour complet (4 quarts de tour)
    const int32_t maxSweepCounts = 4L * currentMotorSettings.rotationTargetCounts;

    float distanceObjet = -1;

    while (distanceObjet < 0)
    {
        ActualiserDel();

        int32_t sweptCounts = (abs(ENCODER_Read(LEFT)) + abs(ENCODER_Read(RIGHT))) / 2;
        if (sweptCounts >= maxSweepCounts)
        {
            break;
        }

        MOTOR_SetSpeed(LEFT, 0.2);
        MOTOR_SetSpeed(RIGHT, -0.2);

        if (obstacleDetecterADistance(distanceMaxM) > 0)
        {
            // Possible objet: s'arreter et confirmer avec plusieurs lectures
            stopMotors();
            delay(100);

            distanceObjet = confirmerObstacle(distanceMaxM);
        }
    }

    stopMotors();
    delay(100); // laisser le robot s'immobiliser avant de lire les encodeurs

    // Sauvegarder la rotation effectuee (inclut le glissement apres l'arret)
    rotationCounts = ((abs(ENCODER_Read(LEFT)) + abs(ENCODER_Read(RIGHT))) / 2) % maxSweepCounts;

    return distanceObjet;
}

// Annule la rotation du balayage puis retrouve la ligne.
static void revenirSurLigne(int32_t rotationCounts)
{
    // On s'arrete ~30 degres avant l'orientation initiale pour que la ligne
    // soit encore a gauche, puis on la cherche avec le capteur de ligne.
    const int32_t margeCounts = currentMotorSettings.rotationTargetCounts / 3; // ~30 degres

    if (rotationCounts > margeCounts)
    {
        rotateCounts(ROTATE_LEFT, rotationCounts - margeCounts);
    }

    // Chercher la ligne a gauche; si on l'a depassee, revenir vers la droite
    if (!chercherLigne(ROTATE_LEFT, 60))
    {
        chercherLigne(ROTATE_RIGHT, 120);
    }

    // Au retour dans loop(), le suivi de ligne fait avancer le robot
}

void FaireTomberQuille()
{
    // La quille est renversee par le deplacement du robot:
    // tourner pour la trouver, foncer dedans, revenir, se realigner.
    int32_t rotationCounts = 0;
    float distanceQuille = balayerVersObjet(QUILLE_DISTANCE_MAX_M, rotationCounts);

    if (distanceQuille > 0)
    {
        // Avancer jusqu'a la quille + 30 cm pour la faire tomber
        float distanceAvancer = distanceQuille + 0.3f;

        moveForward(distanceAvancer);

        // Reculer de la meme distance: pas de demi-tours, donc pas
        // d'erreur de rotation accumulee, on revient pile au depart
        moveForward(-distanceAvancer);
    }

    revenirSurLigne(rotationCounts);
}

void RecupererVerre()
{
  OuvrirPince();
   // La quille est renversee par le deplacement du robot:
    // tourner pour la trouver, foncer dedans, revenir, se realigner.
    int32_t rotationCounts = 0;
    float distanceQuille = balayerVersObjet(QUILLE_DISTANCE_MAX_M, rotationCounts);

    if (distanceQuille > 0)
    {
        // Avancer jusqu'a la quille + 30 cm pour la faire tomber
        float distanceAvancer = distanceQuille;

    
        moveForward(distanceAvancer);
        FermerPince();
        VerreRamasse1 = true;

        // Reculer de la meme distance: pas de demi-tours, donc pas
        // d'erreur de rotation accumulee, on revient pile au depart
        moveForward(-distanceAvancer);
    }

    revenirSurLigne(rotationCounts);

     moveForward(0.3);

}

void DeposerVerre()
{
    if(VerreRamasse1)
    {
        OuvrirPince();
        delay(250);
        VerreRamasse1 = false;
    }
}


bool VerreRamasse(){
    return VerreRamasse1;
}



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
