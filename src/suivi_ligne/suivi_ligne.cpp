#include "suivi_ligne/suivi_ligne.h"

void SUIVI_LIGNE_Initialisation()
{
    pinMode(PIN_CAPTEUR_GAUCHE, INPUT);
    pinMode(PIN_CAPTEUR_CENTRE, INPUT);
    pinMode(PIN_CAPTEUR_DROITE, INPUT);
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}
void SUIVI_LIGNE_Execution()
{
    int g = analogRead(PIN_CAPTEUR_GAUCHE);
    int c = analogRead(PIN_CAPTEUR_CENTRE);
    int d = analogRead(PIN_CAPTEUR_DROITE);

    Serial.print("Gauche: ");
    Serial.print(g);
    Serial.print("   Centre: ");
    Serial.print(c);
    Serial.print("   Droite: ");
    Serial.println(d);
// suivre ligne droite 
//si uniquement le capteur centrale voit la ligne noire
    if (c > SEUIL_NOIR && g < SEUIL_NOIR  && d< SEUIL_NOIR)
    {
        MOTOR_SetSpeed(0, 0.3085);
        MOTOR_SetSpeed(1, 0.3);
    }
       //si les trois voit la ligne noire
    else if (c > SEUIL_NOIR && g > SEUIL_NOIR  && d > SEUIL_NOIR)
    {
        MOTOR_SetSpeed(0, 0.3085);
        MOTOR_SetSpeed(1, 0.3);
    }

    //si capteur centre et gauche voit ligne noire
    else if (c > SEUIL_NOIR && g > SEUIL_NOIR  && d < SEUIL_NOIR)
    {
        MOTOR_SetSpeed(0, 0.2785);
        MOTOR_SetSpeed(1, 0.40);
    }
    //si capteur centre et droite voit ligne noire
    else if (c > SEUIL_NOIR && g < SEUIL_NOIR  && d > SEUIL_NOIR)
    {
        MOTOR_SetSpeed(0, 0.4085);
        MOTOR_SetSpeed(1, 0.27);
    }
//si uniquement le capteur de gauche voit la ligne noire
    else if (c < SEUIL_NOIR && g > SEUIL_NOIR  && d < SEUIL_NOIR)
    {
        MOTOR_SetSpeed(0, 0.0585);
        MOTOR_SetSpeed(1, 0.7);
    }
//si uniquement le capteur de droite voit la ligne noire
    else if (c < SEUIL_NOIR && g < SEUIL_NOIR  && d > SEUIL_NOIR)
    {
        MOTOR_SetSpeed(0, 0.7085);
        MOTOR_SetSpeed(1, 0.05);
    }

   //si aucun voit ligne noire
   
    else if (c < SEUIL_NOIR && g < SEUIL_NOIR  && d < SEUIL_NOIR)
    {
        MOTOR_SetSpeed(0, 0.2085);
        MOTOR_SetSpeed(1, 0.2);
    }


    delay(35);


}