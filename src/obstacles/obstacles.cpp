#include "obstacles/obstacles.h"

// fonction de couleur_del
int color()
{
    return 1;
}
//detecte mur fait un 90 , avance,... pour contourner 
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
//en etant ralentit, selon se qui capte en premier va faire tourner lautre moteur pour calibrer 
//et ensuite avancer pendant 0.75
void tout_droit() 
{
    if (isObstacle())// bon ici faudra changer pout que quand il detecte couleur (comme sa pour test)
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
                else if(g < SEUIL_G && d > SEUIL_D)
                {
                    MOTOR_SetSpeed(0, 0.2);
                    MOTOR_SetSpeed(1, 0); 
                }
                else  
                {
                    SUIVI_LIGNE_Execution(0.20);
                }
            }
            MOTOR_SetSpeed(0,0);
            MOTOR_SetSpeed(1,0);
            moveForward(0.85);
            g = analogRead(PIN_CAPTEUR_GAUCHE);
            c = analogRead(PIN_CAPTEUR_CENTRE);
            d = analogRead(PIN_CAPTEUR_DROITE);
            if (c> SEUIL_C || g > SEUIL_G || d > SEUIL_D)// si il repere la ligne a la fin 
            {
                SUIVI_LIGNE_Execution(1.0);
            }
        }       
    }
void action_obstacle()// fonction qui execute fonction selon couleur detecter 
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