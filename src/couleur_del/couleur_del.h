#ifndef COULEUR_DEL
#define COULEUR_DEL

#include "config.h"

enum CouleurDetectee {
    AUCUNE_COULEUR,
    ROUGE, JAUNE, ORANGE, BLEU, VERT
};

bool InitialiserCouleurDel();
void ActualiserCouleurDel();
CouleurDetectee ObtenirCouleurDetectee();

#endif
