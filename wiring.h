#ifndef WIRING_H
#define WIRING_H

#include <stdint.h>

/*
Hardware wiring definitions
*/

// TODO - Assign actual encoder pins
#define ENCODER_A_PIN 30
#define ENCODER_B_PIN 31
#define MATRIX_PINS_COUNT 24
#define MATRIX_ROW_COUNT 6
#define MATRIX_COL_COUNT 18

static const uint8_t matrix_col_pins[MATRIX_COL_COUNT] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
};

static const uint8_t matrix_row_pins[MATRIX_ROW_COUNT] = {
    18,
    19,
    20,
    21,
    22,
    23,
};


#endif // WIRING_H