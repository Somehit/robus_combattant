#include <motor/motor.h>

MotorSettings currentMotorSettings;

void MotorsInit() {
    ENCODER_ReadReset(LEFT);
    ENCODER_ReadReset(RIGHT);

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    pinMode(ID_PIN, INPUT);

    if (digitalRead(ID_PIN)) {
        currentMotorSettings = MOTOR_SETTINGS_A;
    } else {
        currentMotorSettings = MOTOR_SETTINGS_B;
    }
}

float accelerate(unsigned long elapsed, unsigned long duration, float targetSpeed) {
    if (elapsed >= duration) {
        return targetSpeed;
    }

    // Calculer le progres de l'acceleration entre 0 et 1
    float progress = (float)elapsed / duration;

    // Partir de MIN_SPEED: sous cette vitesse les roues ne bougent presque pas
    return MIN_SPEED + (targetSpeed - MIN_SPEED) * progress;
}

float decelerate(float progress, float targetSpeed) {
    // progress: progres du freinage entre 0 et 1
    if (progress >= 1.0f) {
        return MIN_SPEED;
    }

    float speedDrop = targetSpeed - MIN_SPEED;

    return targetSpeed - (speedDrop * progress);
}

void adjustWheelPID(float kp, float ki, float maxCorrection, float leftTarget, float rightTarget, int32_t leftSpeed, int32_t rightSpeed, int32_t leftTotal, int32_t rightTotal) {
    const float ratio = currentMotorSettings.wheelCountRatio;

    const float speedError = (float)abs(leftSpeed) - (float)abs(rightSpeed) * ratio;
    const float distanceError = (float)abs(leftTotal) - (float)abs(rightTotal) * ratio;

    const float rawCorrection = kp * speedError + ki * distanceError;
    const float correction = constrain(rawCorrection, -maxCorrection, maxCorrection);

    const float leftDirection = (leftTarget < 0.0f) ? -1.0f : 1.0f;
    const float rightDirection = (rightTarget < 0.0f) ? -1.0f : 1.0f;

    float leftMagnitude = max(0.0f, abs(leftTarget) - correction);
    float rightMagnitude = max(0.0f, abs(rightTarget) + correction);

    const float largestMagnitude = max(leftMagnitude, rightMagnitude);

    if (largestMagnitude > 1.0f) {
        const float scale = 1.0f / largestMagnitude;

        leftMagnitude *= scale;
        rightMagnitude *= scale;
    }

    const float newLeftSpeed = leftDirection * leftMagnitude;
    const float newRightSpeed = rightDirection * rightMagnitude;

    MOTOR_SetSpeed(LEFT, constrain(newLeftSpeed, -1.0f, 1.0f));
    MOTOR_SetSpeed(RIGHT, constrain(newRightSpeed, -1.0f, 1.0f));
}

void stopMotors() {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

// Convertit une distance en metres vers des impulsions d'encodeur (avec calibration)
float metersToCounts(float distanceMeters) {
    float distanceCm = distanceMeters * 100.0f;
    float wheelRevolutions = distanceCm / WHEEL_CIRCUMFERENCE_CM;
    float rawEncoderCounts = wheelRevolutions * ENCODER_COUNTS_PER_REVOLUTION;

    return rawEncoderCounts * currentMotorSettings.distanceCalibrationFactor;
}

void moveForward(float distanceMeters) {
    float totalTargetCounts = metersToCounts(distanceMeters);

    // Definir le point ou le robot commence a ralentir
    float decelerationStartCounts = totalTargetCounts * DECELERATION_START_RATIO;

    ENCODER_ReadReset(LEFT);
    ENCODER_ReadReset(RIGHT);

    unsigned long startTimeMs = millis();
    unsigned long lastPIDTimeMs = startTimeMs;

    int32_t previousLeftCount  = ENCODER_Read(LEFT);
    int32_t previousRightCount = ENCODER_Read(RIGHT);

    while (true) {
        unsigned long currentTimeMs = millis();
        unsigned long elapsedTimeMs = currentTimeMs - startTimeMs;

        // Lire les impulsions actuelles des encodeurs
        int32_t currentLeftCount  = ENCODER_Read(LEFT);
        int32_t currentRightCount = ENCODER_Read(RIGHT);

        // Calculer la moyenne des impulsions des deux roues
        float averageDistanceCounts = (abs(currentLeftCount) + abs(currentRightCount)) / 2.0f;

        // Arreter le robot quand la distance cible est atteinte
        if (averageDistanceCounts >= totalTargetCounts) {
            break;
        }

        // Vitesse permise par la rampe d'acceleration (selon le temps)
        float accelSpeed = accelerate(elapsedTimeMs, ACCELERATION_DURATION_MS, TARGET_SPEED);

        // Vitesse permise par la rampe de freinage (selon la distance)
        float decelSpeed = TARGET_SPEED;

        if (averageDistanceCounts >= decelerationStartCounts) {
            float remainingDecelDistance = totalTargetCounts - decelerationStartCounts;
            float distanceDecelerated = averageDistanceCounts - decelerationStartCounts;

            // Calculer le progres du freinage entre 0 et 1
            float decelProgress = distanceDecelerated / remainingDecelDistance;

            decelSpeed = decelerate(decelProgress, TARGET_SPEED);
        }

        // Garder la plus petite des deux vitesses pour que le freinage
        // s'applique meme si l'acceleration n'est pas terminee
        float currentTargetSpeed = min(accelSpeed, decelSpeed);

        // Ajuster les roues regulierement pour garder une trajectoire droite
        unsigned long timeSinceLastPID = currentTimeMs - lastPIDTimeMs;

        if (timeSinceLastPID >= PID_INTERVAL_MS) {
            lastPIDTimeMs = currentTimeMs;

            // Calculer le nombre d'impulsions depuis la derniere correction
            int32_t leftSpeedTicks  = currentLeftCount - previousLeftCount;
            int32_t rightSpeedTicks = currentRightCount - previousRightCount;

            // Sauvegarder les valeurs pour la prochaine correction
            previousLeftCount  = currentLeftCount;
            previousRightCount = currentRightCount;

            // Corriger la vitesse des roues
            adjustWheelPID(currentMotorSettings.pidKp, currentMotorSettings.pidKi, currentMotorSettings.maxPidCorrection,
                           currentTargetSpeed, currentTargetSpeed,
                           leftSpeedTicks, rightSpeedTicks,
                           currentLeftCount, currentRightCount);
        }
    }

    stopMotors();
}

// Ramene chaque roue vers la cible apres l'arret. Sans ca, l'ecart entre
// les roues et le glissement apres l'arret restent a chaque virage et
// s'accumulent, car les encodeurs sont remis a zero au mouvement suivant.
void finishRotation(RotationDirection direction, int32_t targetCounts) {
    // Signe de chaque roue quand elle avance dans le sens de la rotation
    const int32_t leftSign  = (direction == ROTATE_LEFT) ? -1 : 1;
    const int32_t rightSign = -leftSign;

    unsigned long startTimeMs = millis();

    while (millis() - startTimeMs < ROTATION_CORRECTION_TIMEOUT_MS) {
        // Erreur de chaque roue: positive si elle n'a pas assez tourne
        int32_t leftError  = targetCounts - leftSign * ENCODER_Read(LEFT);
        int32_t rightError = targetCounts - rightSign * ENCODER_Read(RIGHT);

        bool leftDone  = abs(leftError) <= ROTATION_TOLERANCE_COUNTS;
        bool rightDone = abs(rightError) <= ROTATION_TOLERANCE_COUNTS;

        if (leftDone && rightDone) {
            // Laisser le robot s'immobiliser, puis verifier qu'il n'a pas glisse
            stopMotors();
            delay(ROTATION_SETTLE_MS);

            leftError  = targetCounts - leftSign * ENCODER_Read(LEFT);
            rightError = targetCounts - rightSign * ENCODER_Read(RIGHT);

            if (abs(leftError) <= ROTATION_TOLERANCE_COUNTS && abs(rightError) <= ROTATION_TOLERANCE_COUNTS) {
                break;
            }

            continue;
        }

        // Pousser lentement chaque roue vers sa cible, ou l'arreter si elle y est
        float leftSpeed  = leftDone ? 0.0f : ((leftError > 0) ? 1.0f : -1.0f) * leftSign * MIN_SPEED;
        float rightSpeed = rightDone ? 0.0f : ((rightError > 0) ? 1.0f : -1.0f) * rightSign * MIN_SPEED;

        MOTOR_SetSpeed(LEFT, leftSpeed);
        MOTOR_SetSpeed(RIGHT, rightSpeed);
    }

    stopMotors();
}

void rotate(RotationDirection direction) {
    // Definir la distance de rotation et le point de freinage
    float totalTargetCounts = currentMotorSettings.rotationTargetCounts;
    float decelerationStartCounts = totalTargetCounts * ROTATION_DECELERATION_START_RATIO;

    ENCODER_ReadReset(LEFT);
    ENCODER_ReadReset(RIGHT);

    unsigned long startTimeMs = millis();
    unsigned long lastPIDTimeMs = startTimeMs;

    int32_t previousLeftCount  = 0;
    int32_t previousRightCount = 0;

    while (true) {
        unsigned long currentTimeMs = millis();
        unsigned long elapsedTimeMs = currentTimeMs - startTimeMs;

        // Lire les impulsions des deux roues
        int32_t currentLeftCount  = abs(ENCODER_Read(LEFT));
        int32_t currentRightCount = abs(ENCODER_Read(RIGHT));

        // Calculer la moyenne des impulsions des deux roues
        float averageDistanceCounts = (currentLeftCount + currentRightCount) / 2.0f;

        // Arreter la rotation quand la cible est atteinte
        if (averageDistanceCounts >= totalTargetCounts) {
            break;
        }

        // Vitesse permise par la rampe d'acceleration (selon le temps)
        float accelSpeed = accelerate(elapsedTimeMs, ROTATION_ACCELERATION_DURATION_MS, ROTATION_SPEED);

        // Vitesse permise par la rampe de freinage (selon la distance)
        float decelSpeed = ROTATION_SPEED;

        if (averageDistanceCounts >= decelerationStartCounts) {
            float totalDecelDistance  = totalTargetCounts - decelerationStartCounts;
            float distanceDecelerated = averageDistanceCounts - decelerationStartCounts;

            // Calculer le progres du freinage entre 0 et 1
            float decelProgress = distanceDecelerated / totalDecelDistance;

            // Ne descend pas sous MIN_SPEED pour eviter que les moteurs calent
            decelSpeed = decelerate(decelProgress, ROTATION_SPEED);
        }

        // Garder la plus petite des deux vitesses
        float currentSpeed = min(accelSpeed, decelSpeed);

        // Definir le sens de chaque roue
        float leftTargetSpeed;
        float rightTargetSpeed;

        if (direction == ROTATE_LEFT) {
            leftTargetSpeed  = -currentSpeed;
            rightTargetSpeed = currentSpeed;
        } else {
            leftTargetSpeed  = currentSpeed;
            rightTargetSpeed = -currentSpeed;
        }

        // Corriger les roues regulierement, aussi pendant l'acceleration
        if (currentTimeMs - lastPIDTimeMs >= PID_INTERVAL_MS) {
            lastPIDTimeMs = currentTimeMs;

            // Calculer le nombre d'impulsions depuis la derniere correction
            int32_t leftSpeedTicks  = currentLeftCount - previousLeftCount;
            int32_t rightSpeedTicks = currentRightCount - previousRightCount;

            // Sauvegarder les valeurs pour la prochaine correction
            previousLeftCount  = currentLeftCount;
            previousRightCount = currentRightCount;

            // Corriger la vitesse des roues pour garder une rotation reguliere
            adjustWheelPID(currentMotorSettings.rotationPidKp, currentMotorSettings.rotationPidKi,
                           currentMotorSettings.rotationMaxPidCorrection,
                           leftTargetSpeed, rightTargetSpeed,
                           leftSpeedTicks, rightSpeedTicks,
                           currentLeftCount, currentRightCount);
        }
    }

    stopMotors();

    finishRotation(direction, (int32_t)totalTargetCounts);
}