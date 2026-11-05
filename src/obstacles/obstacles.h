/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    obstacles.h
 *  @brief   Franchsissmeent des obstacles du parcours
 *
 *  @details contourner un obstacle (devier_obstacle)
 *           et passer le trou tout droit en se réalignant sur la ligne (tout_droit).
 *
 *  @author  Naguy
 *  @date    2026-11-02
 *  @version 1.0
 * ============================================================================
 */

#ifndef OBSTACLES
#define OBSTACLES

#include "config.h"
#include "infrarouge/infrarouge.h"
#include "motor/motor.h"
#include "couleur_del/couleur_del.h"
#include "infrared/infrared.h"   // ou "infrared/infrared/infrared.h" selon ton dossier
#include "suivi_ligne/suivi_ligne.h"

/**
 * @brief choisit l'action selon couleur
 */
void action_obstacle();

// --- CONTOURNEMENT DU MUR (jaune) ---
constexpr float MUR_VITESSE_APPROCHE = 0.40f; // facteur du suivi de ligne vers le mur
constexpr float MUR_DECALAGE_M = 0.30f;       // distance de cote pour eviter le mur
constexpr float MUR_LONGUEUR_M = 0.55f;       // distance pour depasser le mur

/**
 * @brief anciennement dans mur.h, coutourne le mur (doit être appelée quand le robot voit du jaune)
 */
void contournerMur(); 

/**
 * @brief anciennement dans arret.h s'arrete et attend le signal IR (doit etre appelle quand il voit du bleu)
 */
void attendreSignalDepart();

// A FAIRE
//bool messageReveilRecul();



/**
 * @brief Fait avancer le robot tout droit jusqu'à ce qu'il ne détecte plus d'obstacle.
 */
void tout_droit();

/**
 * @brief Detecte un obstacle a une distance donnee
 * @param distanceM La distance a la quelle detectee
 * @return La distance de l'objet qui est toujours moins que distanceM, sinon -1
 */
float obstacleDetecterADistance(float distanceM);

// --- SONAR ---
constexpr uint8_t SONAR_ID = 0;
constexpr unsigned long SONAR_DELAI_PING_MS = 60; // delai minimal entre 2 pings (HC-SR04)
constexpr float SONAR_MIN_M = 0.03f;              // lectures plus courtes = bruit
constexpr uint8_t SONAR_CONFIRMATIONS = 3;        // lectures requises pour confirmer
constexpr float SONAR_TOLERANCE_M = 0.05f;        // ecart max entre les lectures

/**
 * @brief Confirme un obstacle avec plusieurs lectures (robot immobile).
 * @return Distance moyenne en metres si confirme, sinon -1
 */
float confirmerObstacle(float distanceM);

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

// Fin

#endif