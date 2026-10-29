#include "suivi_ligne/suivi_ligne.h"

void SUIVI_LIGNE_Initialisation()
{
    pinMode(PIN_CAPTEUR_GAUCHE, INPUT);
    pinMode(PIN_CAPTEUR_CENTRE, INPUT);
    pinMode(PIN_CAPTEUR_DROITE, INPUT);
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

void SUIVI_LIGNE_Execution(float facteur=0.7)// facteur regle vistesse dun coup et aussi sert pour dautre fonction
{
    int g = analogRead(PIN_CAPTEUR_GAUCHE);
    int c = analogRead(PIN_CAPTEUR_CENTRE);
    int d = analogRead(PIN_CAPTEUR_DROITE);

    // si uniquement le capteur central voit la ligne noire
    if (c > SEUIL_C && g < SEUIL_G && d < SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.5065*facteur);  
        MOTOR_SetSpeed(1, 0.50*facteur);
    }
    // si les trois voient la ligne noire
    else if (c > SEUIL_C && g > SEUIL_G && d > SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.5065*facteur);
        MOTOR_SetSpeed(1, 0.50*facteur);
    }
    // si capteur centre et gauche voit ligne noire
    else if (c > SEUIL_C && g > SEUIL_G && d < SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.4785*facteur);
        MOTOR_SetSpeed(1, 0.60*facteur);
    }
    // si capteur centre et droite voit ligne noire
    else if (c > SEUIL_C && g < SEUIL_G && d > SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.6085*facteur);
        MOTOR_SetSpeed(1, 0.47*facteur);
    }
    // si uniquement le capteur de gauche voit la ligne noire
    else if (c < SEUIL_C && g > SEUIL_G && d < SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.05*facteur);     
        MOTOR_SetSpeed(1, 0.95*facteur);     
    }
    // si uniquement le capteur de droite voit la ligne noire
    else if (c < SEUIL_C && g < SEUIL_G && d > SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.9585*facteur);
        MOTOR_SetSpeed(1, 0.05*facteur);
    }
    // si aucun ne voit la ligne noire
    else if (c < SEUIL_C && g < SEUIL_G && d < SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.4085*facteur);
        MOTOR_SetSpeed(1, 0.4001*facteur);
    }

    delay(15);
}