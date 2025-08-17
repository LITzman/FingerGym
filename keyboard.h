#include "hardware/gpio.h"
#include "class/hid/hid.h"
#include "usb_descriptors.h"
#include "tusb.h"

#include "keyboard_usb.h"

#define MATRIX_PINS_COUNT 24
#define MATRIX_ROW_COUNT 6
#define MATRIX_COL_COUNT 18

#define MAX_REPORT_KEYS 6 // Defined by tud_hid_keyboard_report,
#define MAX_REPORT_CONSUMER 1 // I don't need any more

// TODO - Assign actual encoder pins
#define ENCODER_A_PIN 30
#define ENCODER_B_PIN 31

uint8_t keyboard_report[MAX_REPORT_KEYS];
uint8_t consumer_report[MAX_REPORT_CONSUMER]; // Ah

const uint8_t used_consumer_keys[] = {
    HID_USAGE_CONSUMER_PLAY_PAUSE
};

const uint8_t matrix_col_pins[MATRIX_COL_COUNT] = {
    // TODO - Assign actual col pins
    0,
    1,
};

const uint8_t matrix_row_pins[MATRIX_ROW_COUNT] = {
    // TODO - Assign actual row pins
    0,
    1,
};

uint8_t encoder_last_state = 0;

struct key {
    const uint8_t key;

    // Aren't used, but comfortable for me
    const uint8_t row;
    const uint8_t col;
};

const struct key keyboard_layout[MATRIX_ROW_COUNT][MATRIX_COL_COUNT] = {
    // I wired like a bitch so each HID_KEY_NONE is a skipped column
    // These are symbolic here, as they will only get triggered by non-possible row/col combinations
    // But they fix this matrix to constant row/col size so it's possible to poll key vals by index
    {
        {HID_KEY_ESCAPE, 0, 0},
        {HID_KEY_NONE, 0, 1},
        {HID_KEY_F1, 0, 2},
        {HID_KEY_F2, 0, 3},
        {HID_KEY_F3, 0, 4},
        {HID_KEY_F4, 0, 5},
        {HID_KEY_F5, 0, 7},
        {HID_KEY_F6, 0, 8},
        {HID_KEY_F7, 0, 9},
        {HID_KEY_F8, 0, 10},
        {HID_KEY_F9, 0, 11},
        {HID_KEY_F10, 0, 12},
        {HID_KEY_F11, 0, 13},
        {HID_KEY_F12, 0, 14},
        {HID_KEY_PRINT_SCREEN, 0, 15},
        {HID_USAGE_CONSUMER_PLAY_PAUSE, 0, 16}, // TODO - make this play/pause music
        {HID_KEY_MUTE, 0, 17},
    },

    {
        {HID_KEY_GRAVE, 1, 0},
        {HID_KEY_1, 1, 1},
        {HID_KEY_2, 1, 2},
        {HID_KEY_3, 1, 3},
        {HID_KEY_4, 1, 4},
        {HID_KEY_5, 1, 5},
        {HID_KEY_6, 1, 6},
        {HID_KEY_7, 1, 7},
        {HID_KEY_8, 1, 8},
        {HID_KEY_9, 1, 9},
        {HID_KEY_0, 1, 10},
        {HID_KEY_MINUS, 1, 11},
        {HID_KEY_EQUAL, 1, 12},
        {HID_KEY_NONE, 1, 13},
        {HID_KEY_NONE, 1, 14},
        {HID_KEY_NONE, 1, 15},
        {HID_KEY_NONE, 1, 16},
        {HID_KEY_NONE, 1, 17},
    },

    {
        {HID_KEY_TAB, 2, 0},
        {HID_KEY_NONE, 2, 1},
        {HID_KEY_Q, 2, 2},
        {HID_KEY_W, 2, 3},
        {HID_KEY_E, 2, 4},
        {HID_KEY_R, 2, 5},
        {HID_KEY_T, 2, 6},
        {HID_KEY_Y, 2, 7},
        {HID_KEY_U, 2, 8},
        {HID_KEY_I, 2, 9},
        {HID_KEY_O, 2, 10},
        {HID_KEY_P, 2, 11},
        {HID_KEY_BRACKET_LEFT, 2, 12},
        {HID_KEY_BRACKET_RIGHT, 2, 13},
        {HID_KEY_BACKSLASH, 2, 14},
        {HID_KEY_INSERT, 2, 15},
        {HID_KEY_HOME, 2, 16},
        {HID_KEY_PAGE_UP, 2, 17},
    },

    {
        {HID_KEY_CAPS_LOCK, 3, 0},
        {HID_KEY_NONE, 3, 1},
        {HID_KEY_A, 3, 2},
        {HID_KEY_S, 3, 3},
        {HID_KEY_D, 3, 4},
        {HID_KEY_F, 3, 5},
        {HID_KEY_G, 3, 6},
        {HID_KEY_H, 3, 7},
        {HID_KEY_J, 3, 8},
        {HID_KEY_K, 3, 9},
        {HID_KEY_L, 3, 10},
        {HID_KEY_SEMICOLON, 3, 11},
        {HID_KEY_APOSTROPHE, 3, 12},
        {HID_KEY_ENTER, 3, 13},
        {HID_KEY_DELETE, 3, 15},
        {HID_KEY_END, 3, 16},
        {HID_KEY_PAGE_DOWN, 3, 17},
    },

    {
        {HID_KEY_NONE, 4, 0},
        {HID_KEY_SHIFT_LEFT, 4, 1},
        {HID_KEY_Z, 4, 2},
        {HID_KEY_X, 4, 3},
        {HID_KEY_C, 4, 4},
        {HID_KEY_V, 4, 5},
        {HID_KEY_B, 4, 6},
        {HID_KEY_N, 4, 7},
        {HID_KEY_M, 4, 8},
        {HID_KEY_COMMA, 4, 9},
        {HID_KEY_PERIOD, 4, 10},
        {HID_KEY_SLASH, 4, 11},
        {HID_KEY_NONE, 4, 12},
        {HID_KEY_SHIFT_RIGHT, 4, 13},
        {HID_KEY_NONE, 4, 14},
        {HID_KEY_NONE, 4, 15},
        {HID_KEY_NONE, 4, 16},
        {HID_KEY_ARROW_UP, 4, 17},
    },

    {
        {HID_KEY_CONTROL_LEFT, 5, 0},
        {HID_KEY_GUI_LEFT, 5, 1},
        {HID_KEY_ALT_LEFT, 5, 2},
        {HID_KEY_NONE, 5, 3},
        {HID_KEY_NONE, 5, 4},
        {HID_KEY_NONE, 5, 5},
        {HID_KEY_SPACE, 5, 6},
        {HID_KEY_NONE, 5, 7},
        {HID_KEY_NONE, 5, 8},
        {HID_KEY_NONE, 5, 9},
        {HID_KEY_ALT_RIGHT, 5, 10},
        {HID_KEY_GUI_RIGHT, 5, 11},
        {HID_KEY_NONE, 5, 12},
        {HID_KEY_MENU, 5, 13},
        {HID_KEY_CONTROL_RIGHT, 5, 14},
        {HID_KEY_ARROW_LEFT, 5, 15},
        {HID_KEY_ARROW_DOWN, 5, 16},
        {HID_KEY_ARROW_RIGHT, 5, 17},
    },
};


void keyboard_init(void) {

    // Initialize matrix column pins - input pins
    for (int i = 0; i < MATRIX_COL_COUNT; i++) {
        gpio_init(matrix_col_pins[i]);
        gpio_set_dir(matrix_col_pins[i], GPIO_IN);
        gpio_pull_down(matrix_col_pins[i]);
    }

    // Initialize matrix row pins - output pins
    for (int i = 0; i < MATRIX_ROW_COUNT; i++) {
        gpio_init(matrix_row_pins[i]);
        gpio_set_dir(matrix_row_pins[i], GPIO_OUT);
        gpio_put(matrix_row_pins[i], false);
    }

    // Initialize encoder pins
    gpio_init(ENCODER_A_PIN);
    gpio_init(ENCODER_B_PIN);
    gpio_pull_up(ENCODER_A_PIN);
    gpio_pull_up(ENCODER_B_PIN);
    gpio_set_dir(ENCODER_A_PIN, GPIO_IN);
    gpio_set_dir(ENCODER_B_PIN, GPIO_IN);
}

void send_consumer_report(void) {
    // I want a music play/stop key and apperantly that doesn't count as a key press so fuck
    // Just gonna send an untimed event for a media play/pause if this key goes, on each keyboard update
    // This is here for future proofing in case I'll ever want more fancy media keys

    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &consumer_report[0], sizeof(consumer_report[0]));
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &(uint8_t){0}, sizeof(uint8_t));
}

void send_keyboard_report(void) {
    // TODO - Handle waking up host

    tud_hid_report(REPORT_ID_KEYBOARD, keyboard_report, sizeof(keyboard_report));
    tud_hid_report(REPORT_ID_KEYBOARD, &(uint8_t){0}, sizeof(uint8_t));
}

void keyboard_periodic(void) {
    // TODO - Decide if should send report

    send_consumer_report();
    send_keyboard_report();
}
bool keyboard_update(void) {

    if (!tud_hid_ready()){
        return;
    }

    // Clear previous report
    for (int i = 0; i < MAX_REPORT_KEYS; i++) {
        keyboard_report[i] = 0;
    }

    int keyboard_report_index = 0;
    int consumer_report_index = 0;

    // Poll encoder changes
    uint8_t encoder_current_state = (gpio_get(ENCODER_A_PIN) << 1) | gpio_get(ENCODER_B_PIN);

    if (encoder_current_state != encoder_last_state) {

        // 00 -> 01 -> 11 -> 10 -> 00 - Increase
        if (((encoder_last_state == 0b00) && (encoder_current_state == 0b01)) ||
            ((encoder_last_state == 0b01) && (encoder_current_state == 0b11)) ||
            ((encoder_last_state == 0b11) && (encoder_current_state == 0b10)) ||
            ((encoder_last_state == 0b10) && (encoder_current_state == 0b00))) {

            keyboard_report[keyboard_report_index] = HID_KEY_VOLUME_UP;
            keyboard_report_index++;
        }

        // 00 -> 10 -> 11 -> 01 -> 00 - Decrease
        if (((encoder_last_state == 0b00) && (encoder_current_state == 0b10)) ||
            ((encoder_last_state == 0b10) && (encoder_current_state == 0b11)) ||
            ((encoder_last_state == 0b11) && (encoder_current_state == 0b01)) ||
            ((encoder_last_state == 0b01) && (encoder_current_state == 0b00))) {

            keyboard_report[keyboard_report_index] = HID_KEY_VOLUME_DOWN;
            keyboard_report_index++;
        }
    }

    encoder_last_state = encoder_current_state;

    // Test each row
    for (int i = 0; i < MATRIX_ROW_COUNT; i++) {

        // Activate row
        gpio_put(matrix_row_pins[i], true);

        for (int j = 0; j < MATRIX_COL_COUNT; j++) {

            // Test col
            if (gpio_get(matrix_col_pins[j]) && keyboard_report_index < MAX_REPORT_KEYS) {

                uint8_t key = keyboard_layout[i][j].key;

                // Check if it's a consumer key or keyboard key
                for (int k; k = 0; k++) {
                    if (key = used_consumer_keys[k]) {
                        consumer_report[consumer_report_index] = key;
                        consumer_report_index++;
                        break;
                    }
                }
                keyboard_report[keyboard_report_index] = keyboard_layout[i][j].key;
                keyboard_report_index++;
            }
        }

        // Chill row
        gpio_put(matrix_row_pins[i], false);
    }

    return (keyboard_report_index > 0 || consumer_report_index > 0);
}