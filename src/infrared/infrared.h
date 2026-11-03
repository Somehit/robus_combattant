#ifndef INFRARED_H
#define INFRARED_H

#include <config.h>

constexpr uint8_t GREEN_IR_PIN = 48;
constexpr uint8_t RED_IR_PIN = 49;

/**
 * Configure les broches des capteurs infrarouges vert et rouge en entree.
 */
void InfraredInit();

/**
 * Lit les capteurs infrarouges pour detecter un obstacle devant le robot.
 *
 * @return true si au moins un des deux capteurs detecte un obstacle
 */
bool isObstacle();

#endif