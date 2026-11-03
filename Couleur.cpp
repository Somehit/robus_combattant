
#include "Couleur.h"

#define TCS_ADRESSE 0x29


void ecrireRegistre(uint8_t registre, uint8_t valeur) {
    Wire.beginTransmission(TCS_ADRESSE);
    Wire.write(0x80 | registre);
    Wire.write(valeur);
    Wire.endTransmission();
}


uint16_t lireRegistre16(uint8_t registre) {
    Wire.beginTransmission(TCS_ADRESSE);
    Wire.write(0xA0 | registre);

    if (Wire.endTransmission(false) != 0) {
        return 0;
    }

    if (Wire.requestFrom(TCS_ADRESSE, (uint8_t)2) != 2) {
        return 0;
    }

    uint8_t bas = Wire.read();
    uint8_t haut = Wire.read();

    return ((uint16_t)haut << 8) | bas;
}


bool initCouleur() {
    Wire.begin();

    Wire.beginTransmission(TCS_ADRESSE);
    if (Wire.endTransmission() != 0) {
        return false;
    }

    ecrireRegistre(0x00, 0x01);
    delay(3);

   
    ecrireRegistre(0x01, 0xD5);

    
    ecrireRegistre(0x0F, 0x01);

   
    ecrireRegistre(0x00, 0x03);

    delay(110);
    return true;
}


Couleur detecterCouleur() {

    uint16_t c = lireRegistre16(0x14);
    uint16_t r = lireRegistre16(0x16);
    uint16_t g = lireRegistre16(0x18);
    uint16_t b = lireRegistre16(0x1A);

    if (c < 100 || c >= 42000) {
        return INCONNUE;
    }

    float total = (float)r + g + b;

    if (total <= 0) {
        return INCONNUE;
    }

    float R = r / total;
    float G = g / total;
    float B = b / total;

    
    if (B > 0.43 && B > R * 1.3 && B > G * 1.15) {
        return BLEU;
    }

    
    if (G > 0.43 && G > R * 1.25 && G > B * 1.2) {
        return VERT;
    }

   
    if (R > 0.40 && B > 0.20 &&
        R > G * 1.35 && B > G * 0.8) {
        return ROSE;
    }

    
    if (R > 0.48 && G > 0.22 &&
        G < 0.40 && B < 0.18) {
        return ORANGE;
    }

    
    if (R > 0.38 && G > 0.34 &&
        B < 0.20) {
        return JAUNE;
    }

    return INCONNUE;
}


const char* nomCouleur(Couleur couleur) {

    switch (couleur) {
        case BLEU: return "Bleu";
        case VERT: return "Vert";
        case ORANGE: return "Orange";
        case ROSE: return "Rose";
        case JAUNE: return "Jaune";
        default: return "Inconnue";
    }
}
