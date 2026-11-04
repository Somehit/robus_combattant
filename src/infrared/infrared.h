/**
 * ============================================================================
 *  Robus Combattant — Compétition « Le rectangle »
 * ============================================================================
 *  @file    infrared.h
 *  @brief   Capteurs de proximité IR (broches 48 et 49)
 *
 *  @details Détecter un obstacle devant le robot
 *
 *  @author  Naguy
 *  @date    2026-11-02
 *  @version 1.0
 * ============================================================================
 */

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