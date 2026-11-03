
#ifndef COULEUR_H
#define COULEUR_H

#include <Arduino.h>
#include <Wire.h>

enum Couleur {
    INCONNUE,
    BLEU,
    VERT,
    ORANGE,
    ROSE,
    JAUNE
};

bool initCouleur();
Couleur detecterCouleur();
const char* nomCouleur(Couleur couleur);

#endif
