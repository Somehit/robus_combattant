
/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    couleur_del.h
 *  @brief   Détection de couleur et gestion des DEL
 *
 *  @details Détection des couleurs avec le capteur TCS34725.
 *           Utilise les proportions RGB normalisées pour reconnaître
 *           les couleurs indépendamment de la luminosité.
 *           Chaque DEL reste allumée pendant 1 seconde.
 *           Plusieurs DEL peuvent être allumées simultanément.
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

#define DUREE_DEL 10000UL

/** @brief Couleurs reconnaissables par le capteur. */
enum CouleurDetectee {
    ROUGE,
    JAUNE,
    ORANGE,
    BLEU,
    VERT,
    AUCUNE_COULEUR
};

const float centres[5][3] = {
        {42.2, 29.4, 28.3}, // Rouge (rose)
        {37.1, 39.7, 23.2}, // Jaune
        {43.0, 33.7, 23.3}, // Orange
        {20.2, 37.3, 42.5}, // Bleu
        {27.0, 38.4, 34.6}  // Vert
    };

/**
 * @brief Distance max (au carre) acceptee pour chaque couleur, meme ordre que l'enum.
 * Rouge/jaune/orange sont proches entre eux: on garde 25.
 * Le bleu est loin du blanc et des couleurs chaudes, mais sombre donc plus
 * sensible a l'eclairage: on lui laisse plus de marge.
 */
const float DISTANCE_MAX[5] = {25, 25, 25, 60, 25};

/**
 * @brief La meilleure couleur doit etre au moins ce facteur (au carre) plus proche
 * que la deuxieme, sinon la lecture est ambigue (ex: entre bleu et vert).
 */
constexpr float MARGE_AMBIGUITE = 2.0f;

// Mettre a 1 pour afficher les proportions lues dans le moniteur serie
#define DEBUG_COULEUR 1

/** @brief Broches des DEL dans le même ordre que l'enum. */
constexpr uint8_t BROCHES_LEDS[] = {40, 41, 42, 43, 44};

/**
 * @brief Initialise le capteur TCS34725 et les broches des DEL.
 *
 * Configure les 5 DEL en sortie et démarre la communication I2C.
 *
 * @return true si le capteur est détecté, false sinon.
 */
bool CouleurDelInit();

/**
 * @brief Lit le capteur et retourne la couleur détectée.
 *
 * Utilise les proportions RGB normalisées pour identifier la couleur.
 * Allume automatiquement la DEL correspondante.
 *
 * @return Couleur détectée ou AUCUNE_COULEUR.
 */
CouleurDetectee LireCouleur();

/**
 * @brief Allume la DEL associée à une couleur pendant 1 seconde.
 *
 * Chaque DEL possède son propre chronomètre.
 * Si la DEL est déjà allumée, son chronomètre n'est pas réinitialisé.
 *
 * @param couleur Couleur dont on veut allumer la DEL.
 */
void AllumerDel(CouleurDetectee couleur);

/**
 * @brief Éteint individuellement les DEL dont le délai est écoulé.
 *
 * Fonction non bloquante à appeler régulièrement dans loop().
 */
void ActualiserDel();

/**
 * @brief Éteint immédiatement toutes les DEL.
 */
void EteindreDels();

#endif
