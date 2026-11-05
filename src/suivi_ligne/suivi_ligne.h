/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    suivi_lignes.h
 *  @brief   Suivi de ligne avec 3 capteurs de réflexion gauche centre droit sur A0 à A2
 *
 *  @details Fajuste les moteurs pour rester sur la ligne
 *
 *  @author  Charles 
 *  @date    2026-11-02
 *  @version 1.0
 * ============================================================================
 */
#ifndef SUIVI_LIGNE
#define SUIVI_LIGNE

#include "config.h"
#include "motor/motor.h"

// --- CONSTANTES DU MODULE ---
constexpr uint8_t PIN_CAPTEUR_GAUCHE = A2; //U3
constexpr uint8_t PIN_CAPTEUR_CENTRE = A1; //U2
constexpr uint8_t PIN_CAPTEUR_DROITE = A0; //U1
// Seuils de depart (mesures jeudi). Ils servent tant que l'auto-calibration
// n'a pas assez de mesures, puis les seuils s'ajustent tout seuls.
constexpr int SEUIL_C_DEFAUT =150;// seuil depend vraiment de la reflexion de la ligne, selon eclairage et tape
constexpr int SEUIL_G_DEFAUT =140;//chacun des capteur en a des different (valeur noire+ valeur blanc/2)
constexpr int SEUIL_D_DEFAUT =200;//ici selon jeudi, trois ligne diff donnait des truc diff alors jai fait leur moyenne

// --- VIRAGE RATE (ligne perdue) ---
constexpr unsigned long TEMPS_VIRAGE_PERDU_MS = 800; // duree max a pivoter avant de reprendre tout droit
constexpr float VITESSE_PIVOT_EXT = 0.80f;          // roue exterieure (multipliee par facteur)
constexpr float VITESSE_PIVOT_INT = 0.20f;          // roue interieure en marche arriere

// --- AUTO-CALIBRATION ---
constexpr float CALIB_VITESSE = 0.02f;   // vitesse d'adaptation des moyennes (petit = plus stable)
constexpr uint8_t CALIB_MIN_MESURES = 20; // mesures noires ET blanches requises avant d'adapter

// Seuils actuels (ajustes automatiquement pendant SUIVI_LIGNE_Execution)
extern int SEUIL_C;
extern int SEUIL_G;
extern int SEUIL_D;

/**
 * @brief initialiser suiveur de ligne
 */
void SuiviLigneInit();

/**
 *  @brief suivre la ligne
 */
void SUIVI_LIGNE_Execution(float facteur);

// --- RECHERCHE DE LIGNE (apres une manoeuvre) ---
constexpr float VITESSE_RECHERCHE_LIGNE = 0.20f;      // rotation sur place (MIN_SPEED est trop faible)
constexpr float VITESSE_APPROCHE_LIGNE = 0.25f;       // avance lente vers la ligne
constexpr unsigned long TIMEOUT_RECHERCHE_MS = 4000;  // securite: ne jamais bloquer
constexpr uint8_t CONFIRMATIONS_LIGNE = 2;            // lectures consecutives requises
constexpr float DISTANCE_CAPTEURS_AXE_M = 0.08f;      // A MESURER: capteurs de ligne -> axe des roues

/**
 * @brief Tourne sur place jusqu'a ce que le capteur du centre voie la ligne.
 * @param direction Sens de rotation
 * @param maxDegres Angle maximal a balayer
 * @return true si la ligne a ete trouvee
 */
bool chercherLigne(RotationDirection direction, float maxDegres);

/**
 * @brief Avance lentement jusqu'a ce qu'un des capteurs voie la ligne.
 * @param distanceMaxM Distance maximale a parcourir
 * @return true si la ligne a ete trouvee
 */
bool avancerJusquaLigne(float distanceMaxM);

#endif
/*testage capteur ligne (copier sa dans le main en metant le reste en commentaire pour tester seuil)
#include "config.h"

#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrarouge/infrarouge.h"
#include "mecanisme/mecanisme.h"
#include "obstacles/obstacles.h"
#include "suivi_ligne/suivi_ligne.h"

void setup() {
    Serial.begin(9600);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
}

void loop() {
    int g = analogRead(PIN_CAPTEUR_GAUCHE); // A2
    int c = analogRead(PIN_CAPTEUR_CENTRE); // A1
    int d = analogRead(PIN_CAPTEUR_DROITE); // A0

    Serial.print("Gauche: ");
    Serial.print(g);
    Serial.print("   Centre: ");
    Serial.print(c);
    Serial.print("   Droite: ");
    Serial.println(d);

    delay(200);
}*/