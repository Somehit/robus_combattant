/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    couleur_del.h
 *  @brief   Détection de la couleur au sol
 *
 *  @details detection de la couleur du sol avec le Capteur TCS34725
 *           allumage pendant 10 s de la DEL qui correspond à la broches 9 à 13 
 *
 *  @author  Francois
 *  @date    2026-11-02
 *  @version 1.0
 * ============================================================================
 */

#ifndef COULEUR_DEL_H
#define COULEUR_DEL_H

#include "config.h"
#include <Adafruit_TCS34725.h>

/** @brief Couleurs reconnaissables par le capteur (l'ordre correspond à BROCHES_LEDS). */
enum CouleurDetectee {
    ROUGE,
    JAUNE,
    ORANGE,
    BLEU,
    VERT,
    AUCUNE_COULEUR
};

CouleurDetectee CouleurActuelle1 = AUCUNE_COULEUR;

CouleurDetectee CouleurActuelle();

/** @brief Broches des DEL, dans le même ordre que l'enum CouleurDetectee. (RJOBV)*/
constexpr uint8_t BROCHES_LEDS[] = {40, 41, 42, 43, 44};

/** @brief Durée d'allumage d'une DEL en millisecondes. */
constexpr unsigned long DUREE_DEL = 10000;

/** @brief Seuils RGB minimums par couleur {R, G, B}. */
constexpr float SEUILS_MIN[5][3] = {
    {130, 65, 45}, // Rouge/Rose
    {110, 85, 35}, // Jaune
    {130, 70, 35}, // Orange
    {62, 90, 75},  // Bleu
    {80, 90, 55}   // Vert
};

/** @brief Seuils RGB maximums par couleur {R, G, B}. */
constexpr float SEUILS_MAX[5][3] = {
    {140, 75, 55},   // Rouge/Rose
    {120, 95, 45},   // Jaune
    {140, 80, 45},   // Orange
    {72, 100, 85},   // Bleu
    {90, 100, 65}    // Vert
};

/**
 * @brief Initialise le capteur TCS34725 et les broches des DEL.
 *
 * Configure les 5 DEL en sortie (éteintes) et démarre la communication I2C.
 * À appeler une seule fois dans setup().
 *
 * @return bool true si le capteur est détecté, false sinon.
 */
bool CouleurDelInit();

/**
 * @brief Lit le capteur et retourne la couleur détectée.
 *
 * Compare les valeurs R, G, B mesurées aux seuils SEUILS_MIN / SEUILS_MAX.
 * Cette fonction est bloquante pendant le temps d'intégration du capteur (~50 ms).
 * Elle n'allume aucune DEL.
 *
 * @return CouleurDetectee La couleur reconnue, ou AUCUNE_COULEUR si aucune
 *         couleur (ou plusieurs à la fois) ne correspond, ou si le capteur est absent.
 */
CouleurDetectee LireCouleur();

/**
 * @brief Allume la DEL associée à une couleur pendant DUREE_DEL.
 *
 * Ne fait rien si la couleur est AUCUNE_COULEUR ou si une DEL est déjà allumée
 * (elle doit d'abord s'éteindre). L'extinction est gérée par ActualiserDel().
 *
 * @param couleur Couleur dont on veut allumer la DEL.
 */
void AllumerDel(CouleurDetectee couleur);

/**
 * @brief Éteint la DEL une fois DUREE_DEL écoulée (non bloquant).
 *
 * À appeler à chaque tour de loop().
 */
void ActualiserDel();

/**
 * @brief Éteint immédiatement toutes les DEL.
 */
void EteindreDels();

/**
 * @brief Scan la couleur et écris la couleur trouver sur la variable CouleurActuelle,
 *        Allume la led correspondant a la couleur
 *        Éteind la led apres 10sec
 */
void SequenceDels();

#endif
