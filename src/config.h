#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <LibRobus.h>

enum Orientation : uint8_t {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct CurrentPosition {
    int row;
    int column;
    Orientation direction;
};

enum Action : uint8_t {
    MOVE_FORWARD,
    TURN_LEFT,
    TURN_RIGHT
};

const uint8_t COLUMNS = 3;
const uint8_t ROWS = 10;

inline Orientation leftOf(Orientation direction) {
    return (Orientation)((direction + 3) % 4);
}

inline Orientation rightOf(Orientation direction) {
    return (Orientation)((direction + 1) % 4);
}

#endif