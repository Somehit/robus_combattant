#include "suivi_ligne/suivi_ligne.h"

int SEUIL_C = SEUIL_C_DEFAUT;
int SEUIL_G = SEUIL_G_DEFAUT;
int SEUIL_D = SEUIL_D_DEFAUT;

// Moyennes des valeurs "noir" et "blanc" vues par un capteur
struct Calibration
{
    float moyNoir;
    float moyBlanc;
    uint8_t nbNoir;
    uint8_t nbBlanc;
};

static Calibration calibG = {0, 0, 0, 0};
static Calibration calibC = {0, 0, 0, 0};
static Calibration calibD = {0, 0, 0, 0};

// Classe la lecture (noir/blanc) avec le seuil actuel, met a jour la moyenne
// de cette classe, puis place le seuil au milieu entre noir et blanc.
// Le seuil reste le seuil par defaut tant qu'on n'a pas vu assez de noir ET de blanc.
static void calibrer(int valeur, Calibration &cal, int &seuil, int seuilDefaut)
{
    if (valeur > seuil)
    {
        cal.moyNoir = (cal.nbNoir == 0) ? valeur : cal.moyNoir + CALIB_VITESSE * (valeur - cal.moyNoir);
        if (cal.nbNoir < CALIB_MIN_MESURES) cal.nbNoir++;
    }
    else
    {
        cal.moyBlanc = (cal.nbBlanc == 0) ? valeur : cal.moyBlanc + CALIB_VITESSE * (valeur - cal.moyBlanc);
        if (cal.nbBlanc < CALIB_MIN_MESURES) cal.nbBlanc++;
    }

    if (cal.nbNoir >= CALIB_MIN_MESURES && cal.nbBlanc >= CALIB_MIN_MESURES)
    {
        int nouveauSeuil = (int)((cal.moyNoir + cal.moyBlanc) / 2.0f);

        // Securite: le seuil ne peut pas trop s'eloigner de la valeur mesuree a la main
        seuil = constrain(nouveauSeuil, seuilDefaut / 2, seuilDefaut * 2);
    }
}

void SuiviLigneInit()
{
    pinMode(PIN_CAPTEUR_GAUCHE, INPUT);
    pinMode(PIN_CAPTEUR_CENTRE, INPUT);
    pinMode(PIN_CAPTEUR_DROITE, INPUT);
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

// Dernier capteur de cote qui a vu la ligne seul: -1 = gauche, 1 = droite, 0 = aucun
static int8_t dernierCote = 0;
static unsigned long dernierTempsLigneMs = 0;

void SUIVI_LIGNE_Execution(float facteur)// facteur regle vistesse dun coup et aussi sert pour dautre fonction
{
    int g = analogRead(PIN_CAPTEUR_GAUCHE);
    int c = analogRead(PIN_CAPTEUR_CENTRE);
    int d = analogRead(PIN_CAPTEUR_DROITE);

    // Ajuster les seuils selon l'eclairage et le ruban actuels
    calibrer(g, calibG, SEUIL_G, SEUIL_G_DEFAUT);
    calibrer(c, calibC, SEUIL_C, SEUIL_C_DEFAUT);
    calibrer(d, calibD, SEUIL_D, SEUIL_D_DEFAUT);

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
        dernierCote = -1; // on est dans un virage a gauche
    }
    // si uniquement le capteur de droite voit la ligne noire
    else if (c < SEUIL_C && g < SEUIL_G && d > SEUIL_D)
    {
        MOTOR_SetSpeed(0, 0.9585*facteur);
        MOTOR_SetSpeed(1, 0.05*facteur);
        dernierCote = 1; // on est dans un virage a droite
    }
    // si aucun ne voit la ligne noire
    else if (c < SEUIL_C && g < SEUIL_G && d < SEUIL_D)
    {
        bool perdueRecemment = (millis() - dernierTempsLigneMs) < TEMPS_VIRAGE_PERDU_MS;

        if (perdueRecemment && dernierCote == -1)
        {
            // Virage rate par la gauche: pivoter a gauche pour retrouver la ligne
            MOTOR_SetSpeed(0, -VITESSE_PIVOT_INT*facteur);
            MOTOR_SetSpeed(1, VITESSE_PIVOT_EXT*facteur);
        }
        else if (perdueRecemment && dernierCote == 1)
        {
            // Virage rate par la droite: pivoter a droite pour retrouver la ligne
            MOTOR_SetSpeed(0, VITESSE_PIVOT_EXT*facteur);
            MOTOR_SetSpeed(1, -VITESSE_PIVOT_INT*facteur);
        }
        else
        {
            // Comportement d'origine: avancer droit
            MOTOR_SetSpeed(0, 0.4085*facteur);
            MOTOR_SetSpeed(1, 0.4001*facteur);
        }
    }

    // Le centre seul (ou les trois) = on est droit sur la ligne: oublier le virage
    if (c > SEUIL_C && ((g < SEUIL_G && d < SEUIL_D) || (g > SEUIL_G && d > SEUIL_D)))
    {
        dernierCote = 0;
    }

    if (c > SEUIL_C || g > SEUIL_G || d > SEUIL_D)
    {
        dernierTempsLigneMs = millis();
    }

    delay(10);
}

bool chercherLigne(RotationDirection direction, float maxDegres)
{
    ENCODER_ReadReset(LEFT);
    ENCODER_ReadReset(RIGHT);

    const int32_t maxCounts = (int32_t)(currentMotorSettings.rotationTargetCounts * (maxDegres / 90.0f));
    const float sens = (direction == ROTATE_LEFT) ? -1.0f : 1.0f;
    const unsigned long debut = millis();
    uint8_t confirmations = 0;

    while (millis() - debut < TIMEOUT_RECHERCHE_MS)
    {
        // Exiger quelques lectures de suite pour ignorer le bruit
        if (analogRead(PIN_CAPTEUR_CENTRE) > SEUIL_C)
        {
            if (++confirmations >= CONFIRMATIONS_LIGNE)
            {
                stopMotors();
                return true;
            }
        }
        else
        {
            confirmations = 0;
        }

        int32_t counts = (abs(ENCODER_Read(LEFT)) + abs(ENCODER_Read(RIGHT))) / 2;
        if (counts >= maxCounts)
        {
            break;
        }

        MOTOR_SetSpeed(LEFT, sens * VITESSE_RECHERCHE_LIGNE);
        MOTOR_SetSpeed(RIGHT, -sens * VITESSE_RECHERCHE_LIGNE);
    }

    stopMotors();
    return false;
}

bool avancerJusquaLigne(float distanceMaxM)
{
    ENCODER_ReadReset(LEFT);
    ENCODER_ReadReset(RIGHT);

    const float maxCounts = metersToCounts(distanceMaxM);
    const unsigned long debut = millis();
    uint8_t confirmations = 0;

    while (millis() - debut < TIMEOUT_RECHERCHE_MS)
    {
        bool ligne = analogRead(PIN_CAPTEUR_GAUCHE) > SEUIL_G ||
                     analogRead(PIN_CAPTEUR_CENTRE) > SEUIL_C ||
                     analogRead(PIN_CAPTEUR_DROITE) > SEUIL_D;

        if (ligne)
        {
            if (++confirmations >= CONFIRMATIONS_LIGNE)
            {
                stopMotors();
                return true;
            }
        }
        else
        {
            confirmations = 0;
        }

        float counts = (abs(ENCODER_Read(LEFT)) + abs(ENCODER_Read(RIGHT))) / 2.0f;
        if (counts >= maxCounts)
        {
            break;
        }

        MOTOR_SetSpeed(LEFT, VITESSE_APPROCHE_LIGNE);
        MOTOR_SetSpeed(RIGHT, VITESSE_APPROCHE_LIGNE);
    }

    stopMotors();
    return false;
}
