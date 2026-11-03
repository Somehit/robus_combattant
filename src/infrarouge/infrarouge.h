#ifndef INFRAROUGE
#define INFRAROUGE

#include "config.h"
#include <IRremote.hpp>

// Broche du recepteur infrarouge
constexpr uint8_t IR_RECEIVER_PIN = 23;

// Commande qui permet de reveiller le robot
constexpr uint8_t COMMANDE_REVEIL = 0x10;

// Fonctions disponibles
void InfrarougeInit();
bool InfrarougeDetection();

