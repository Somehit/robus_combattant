#ifndef WISTLE_H
#define WISTLE_H

#include <config.h>

constexpr uint8_t WISTLE_PIN = 22;
constexpr uint8_t ANALOG_INPUT_1 = A0;
constexpr uint8_t ANALOG_INPUT_2 = A1;
constexpr uint8_t THRESHOLD = 50;

/**
 * Configure les broches du detecteur de sifflet.
 */
void WistleInit();

/**
 * Lit le detecteur de sifflet pour detecter un sifflet.
 *
 * @return true si un sifflet a sonner
 */
bool isWistleBlown();

#endif