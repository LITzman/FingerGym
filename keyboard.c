#include "keyboard.h"

// Global stuff
static uint8_t keyboard_report[MAX_REPORT_KEYS];
static uint8_t consumer_report[MAX_REPORT_CONSUMER]; // Ah

static bool clear_keyboard = false;
static uint8_t encoder_last_state = 0;

uint32_t led_interval = BLINK_NOT_MOUNTED;

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

bool keyboard_update(void) {

    if (!tud_hid_ready()){
        return false;
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

            if (keyboard_report_index < MAX_REPORT_KEYS) {
                keyboard_report[keyboard_report_index] = HID_KEY_VOLUME_UP;
                keyboard_report_index++;
            }
        }

        // 00 -> 10 -> 11 -> 01 -> 00 - Decrease
        if (((encoder_last_state == 0b00) && (encoder_current_state == 0b10)) ||
            ((encoder_last_state == 0b10) && (encoder_current_state == 0b11)) ||
            ((encoder_last_state == 0b11) && (encoder_current_state == 0b01)) ||
            ((encoder_last_state == 0b01) && (encoder_current_state == 0b00))) {

            if (keyboard_report_index < MAX_REPORT_KEYS) {
                keyboard_report[keyboard_report_index] = HID_KEY_VOLUME_DOWN;
                keyboard_report_index++;
            }
        }
    }

    encoder_last_state = encoder_current_state;

    // Test each row
    for (int i = 0; i < MATRIX_ROW_COUNT; i++) {

        // Activate row
        gpio_put(matrix_row_pins[i], true);

        for (int j = 0; j < MATRIX_COL_COUNT; j++) {

            // Test col
            if (!gpio_get(matrix_col_pins[j])) {
                continue;
            }

            uint8_t key = keyboard_layout[i][j].key;

            if (key == HID_KEY_NONE) {
                continue;
            }

            // Check if it's a consumer key or keyboard key
            bool is_consumer_key = false;
            for (int k = 0; k < sizeof(used_consumer_keys) / sizeof(uint8_t); k++) {
                if (key == used_consumer_keys[k]) {
                    if (consumer_report_index < MAX_REPORT_CONSUMER) {
                        consumer_report[consumer_report_index] = key;
                        consumer_report_index++;
                    }
                    is_consumer_key = true;
                    break;
                }
            }

            if (!is_consumer_key && keyboard_report_index < MAX_REPORT_KEYS) {
                keyboard_report[keyboard_report_index] = key;
                keyboard_report_index++;
            }
        }

        // Chill row
        gpio_put(matrix_row_pins[i], false);
    }

    uint8_t empty = 0;

    // Send reports
    if (keyboard_report_index > 0) {
        tud_hid_report(REPORT_ID_KEYBOARD, keyboard_report, sizeof(keyboard_report));
        clear_keyboard = true;
    } else if (clear_keyboard) {
        tud_hid_report(REPORT_ID_KEYBOARD, &empty, 1);
        clear_keyboard = false;
    }

    if (consumer_report_index > 0) {
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, consumer_report, sizeof(consumer_report));
    } else {
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty, 1);
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &(uint8_t){0}, sizeof(uint8_t));
    }

    // Return true if any key was pressed
    return (keyboard_report_index > 0 || consumer_report_index > 0);
}
