#ifndef COULEUR_DEL_H
#define COULEUR_DEL_H

#include "config.h"

enum CouleurDetectee {
    ROUGE,
    JAUNE,
    ORANGE,
    BLEU,
    VERT,
    AUCUNE_COULEUR
};

const uint8_t BROCHES_LEDS[] = {9, 10, 11, 12, 13};

const unsigned long INTERVALLE_LECTURE = 100;
const unsigned long DUREE_DEL = 10000;

// Seuils RGB minimums
const float SEUILS_MIN[5][3] = {
    {130, 64, 45}, // Rouge/Rose
    {107, 85, 34}, // Jaune
    {131, 72, 31}, // Orange
    {53, 89, 76},  // Bleu
    {79, 93, 61}   // Vert
};


// Seuils RGB maximums
const float SEUILS_MAX[5][3] = {
    {139, 71, 51},   // Rouge/Rose
    {120, 97, 48},   // Jaune
    {142, 79, 44},   // Orange
    {61, 100, 93},   // Bleu
    {91, 101, 71}    // Vert
};

bool InitialiserCouleurDel();
void ActualiserCouleurDel();
CouleurDetectee ObtenirCouleurDetectee();

#endif
