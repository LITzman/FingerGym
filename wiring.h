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

static const uint8_t matrix_col_pins[MATRIX_COL_COUNT];

static const uint8_t matrix_row_pins[MATRIX_ROW_COUNT];


#endif // WIRING_H