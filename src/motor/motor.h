#ifndef MOTOR_H
#define MOTOR_H

#include <config.h>
#include <infrared/infrared.h>

struct MotorSettings {
    float pidKp;
    float pidKi;
    // Impulsions de la roue gauche voulues pour chaque impulsion de la roue droite
    // > 1.0 si le robot derive a gauche, < 1.0 s'il derive a droite
    float wheelCountRatio;
    float maxPidCorrection;
    // Gains separes pour la rotation sur place
    float rotationPidKp;
    float rotationPidKi;
    float rotationMaxPidCorrection;
    float distanceCalibrationFactor;
    uint16_t rotationTargetCounts;
};

enum RotationDirection : uint8_t {
    ROTATE_LEFT,
    ROTATE_RIGHT
};

constexpr uint8_t ID_PIN = 28;

constexpr unsigned long ACCELERATION_DURATION_MS = 1000;
constexpr float DECELERATION_START_RATIO = 0.50f;
constexpr float TARGET_SPEED = 0.60f;
constexpr float MIN_SPEED = 0.15f;
constexpr float BACKWARD_SPEED = 0.25f;

constexpr unsigned long PID_INTERVAL_MS = 5;
constexpr uint16_t ENCODER_COUNTS_PER_REVOLUTION = 3200;
constexpr float WHEEL_CIRCUMFERENCE_CM = 23.876104f;

constexpr unsigned long ROTATION_ACCELERATION_DURATION_MS = 600;
constexpr float ROTATION_DECELERATION_START_RATIO = 0.50f;
constexpr float ROTATION_SPEED = 0.60f;

// Fin de rotation: chaque roue est ramenee vers sa propre cible
constexpr unsigned long ROTATION_SETTLE_MS = 100;
constexpr unsigned long ROTATION_CORRECTION_TIMEOUT_MS = 1000;
constexpr uint16_t ROTATION_TOLERANCE_COUNTS = 25;

constexpr MotorSettings MOTOR_SETTINGS_A = {
    .pidKp = 0.0035f,
    .pidKi = 0.0005f,
    .wheelCountRatio = 1.0f,
    .maxPidCorrection = 0.10f,
    .rotationPidKp = 0.0035f,
    .rotationPidKi = 0.0015f,
    .rotationMaxPidCorrection = 0.15f,
    .distanceCalibrationFactor = 1.0f,
    .rotationTargetCounts = 2061,
};

constexpr MotorSettings MOTOR_SETTINGS_B = {
    .pidKp = 0.0035f,
    .pidKi = 0.0005f,
    .wheelCountRatio = 1.0f,
    .maxPidCorrection = 0.10f,
    .rotationPidKp = 0.0035f,
    .rotationPidKi = 0.0015f,
    .rotationMaxPidCorrection = 0.15f,
    .distanceCalibrationFactor = 1.0f,
    .rotationTargetCounts = 2008,
};

/**
 * Remet les encodeurs a zero, arrete les moteurs et choisit les
 * reglages (A ou B) selon la broche d'identification du robot.
 */
void MotorsInit();

/**
 * Avance en ligne droite avec acceleration, correction PID et deceleration.
 * Bloque jusqu'a ce que la distance soit parcourue ou qu'un obstacle soit detecte.
 *
 * @param distanceMeters Distance a parcourir en metres
 */
void moveForward(float distanceMeters);

/**
 * Tourne le robot sur place d'un quart de tour.
 * Bloque jusqu'a ce que la rotation soit terminee.
 *
 * @param direction Sens de rotation (ROTATE_LEFT ou ROTATE_RIGHT)
 */
void rotate(RotationDirection direction);

#endif
