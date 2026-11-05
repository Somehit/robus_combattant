#include "obstacles/obstacles.h"

// fonction de couleur_del
int color()
{
    return 1;
}

// detecte mur fait un 90 , avance,... pour contourner
void devier_obstacle()
{
    if (isObstacle())
    {
        moveForward(0.5);
        rotate(ROTATE_LEFT);
        moveForward(0.5);
        rotate(ROTATE_LEFT);
        moveForward(0.5);
        rotate(ROTATE_RIGHT);
    }
}

// dans parcour avant le vide il ya une ligne horizontale, le robot ralentit quand detecte couleur
// en etant ralentit, selon se qui capte en premier va faire tourner lautre moteur pour calibrer
// et ensuite avancer pendant 0.75
void tout_droit()
{
    if (LireCouleur() == VERT) // bon ici faudra changer pout que quand il detecte couleur (comme sa pour test)
    {
        int g = analogRead(PIN_CAPTEUR_GAUCHE);
        int c = analogRead(PIN_CAPTEUR_CENTRE);
        int d = analogRead(PIN_CAPTEUR_DROITE);
        while (!(c > SEUIL_C && g > SEUIL_G && d > SEUIL_D))

        {
            g = analogRead(PIN_CAPTEUR_GAUCHE);
            c = analogRead(PIN_CAPTEUR_CENTRE);
            d = analogRead(PIN_CAPTEUR_DROITE);
            if (g > SEUIL_G && d < SEUIL_D)
            {
                MOTOR_SetSpeed(0, 0);
                MOTOR_SetSpeed(1, 0.2);
            }
            else if (g < SEUIL_G && d > SEUIL_D)
            {
                MOTOR_SetSpeed(0, 0.2);
                MOTOR_SetSpeed(1, 0);
            }
            else
            {
                SUIVI_LIGNE_Execution(0.20);
            }
        }
        
        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        moveForward(0.85);
        g = analogRead(PIN_CAPTEUR_GAUCHE);
        c = analogRead(PIN_CAPTEUR_CENTRE);
        d = analogRead(PIN_CAPTEUR_DROITE);
        if (c > SEUIL_C || g > SEUIL_G || d > SEUIL_D) // si il repere la ligne a la fin
        {
            SUIVI_LIGNE_Execution(1.0);
        }
    }
}

void action_obstacle() // fonction qui execute fonction selon couleur detecter
{
    int color1 = color();
    if (color1 == 0)
    {
        devier_obstacle();
    }
    if (color1 == 1)
    {
        tout_droit();
    }
}

void contournerMur()
{
    // S'approcher du mur EN SUIVANT LA LIGNE pour arriver bien droit
    while (!isObstacle())
    {
        ActualiserDel();
        SUIVI_LIGNE_Execution(MUR_VITESSE_APPROCHE);
    }

    stopMotors();
    delay(100);

    // Contourner par la droite
    rotate(ROTATE_RIGHT);
    moveForward(MUR_DECALAGE_M + 0.5);

    rotate(ROTATE_LEFT);
    moveForward(MUR_LONGUEUR_M + 0.5);

    rotate(ROTATE_LEFT);

    // Revenir vers la ligne jusqu'a la voir (au lieu d'une distance fixe)
    if (avancerJusquaLigne(MUR_DECALAGE_M + 0.20f))
    {
        // Les capteurs sont devant les roues: avancer pour que le centre
        // de rotation soit sur la ligne avant de tourner
        moveForward(DISTANCE_CAPTEURS_AXE_M);
    }

    // Tourner a droite en s'arretant ~30 degres avant, puis chercher la ligne
    rotateDegrees(60);

    if (!chercherLigne(ROTATE_RIGHT, 60))
    {
        chercherLigne(ROTATE_LEFT, 120);
    }
    // Au retour dans loop(), le suivi de ligne reprend
}

void attendreSignalDepart()
{

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);

    while (!appuiDetecte(wake))
    {
        delay(100);
    }

    // while (!messageReveilRecul()) {
    //     delay(10);
    // }

    for (int i = 0; i < 3; i++)
    {
        // beep();
        delay(150);
    }
}

// Une lecture du sonar en metres. Attend au moins SONAR_DELAI_PING_MS depuis
// le ping precedent: sinon l'echo de l'ancien ping est lu comme un objet proche.
static float lireSonarM()
{
    static unsigned long dernierPingMs = 0;

    unsigned long ecoule = millis() - dernierPingMs;
    if (ecoule < SONAR_DELAI_PING_MS)
    {
        delay(SONAR_DELAI_PING_MS - ecoule);
    }

    float distanceCm = SONAR_GetRange(SONAR_ID);
    dernierPingMs = millis();

    return distanceCm / 100.0f;
}

float obstacleDetecterADistance(float distanceM)
{
    float distanceLueM = lireSonarM();

    // Sous SONAR_MIN_M (ou 0 = pas d'echo): lecture invalide
    if (distanceLueM >= SONAR_MIN_M && distanceLueM <= distanceM)
    {
        return distanceLueM;
    }

    return -1;
}

float confirmerObstacle(float distanceM)
{
    float somme = 0.0f;
    float premiere = -1.0f;

    for (uint8_t i = 0; i < SONAR_CONFIRMATIONS; i++)
    {
        float d = obstacleDetecterADistance(distanceM);

        if (d < 0)
        {
            return -1; // une lecture sans objet: fausse detection
        }

        if (premiere < 0)
        {
            premiere = d;
        }
        else if (fabs(d - premiere) > SONAR_TOLERANCE_M)
        {
            return -1; // lectures incoherentes: fausse detection
        }

        somme += d;
    }

    return somme / SONAR_CONFIRMATIONS;
}
