#ifndef WIRING_H
#define WIRING_H

#include <stdint.h>

/*
Hardware wiring definitions
*/

// TODO - Assign actual encoder pins
#define ENCODER_A_PIN 0
#define ENCODER_B_PIN 1
#define MATRIX_PINS_COUNT 24
#define MATRIX_ROW_COUNT 6
#define MATRIX_COL_COUNT 18

static const uint8_t matrix_col_pins[MATRIX_COL_COUNT] = {
    3, // 0
    4, // 1
    5, // 2
    6, // 3
    7, // 4
    8, // 5
    9, // 6
    10, // 7
    11, // 8
    12, // 9
    16, // 10
    17, // 11
    14, // 12
    18, // 13
    19, // 14
    15, // 15
    13, // 16
    2, // 17
};

static const uint8_t matrix_row_pins[MATRIX_ROW_COUNT] = {
    27, // 0
    28, // 1
    26, // 2
    22, // 3
    21, // 4
    20, // 5
};


#endif // WIRING_H