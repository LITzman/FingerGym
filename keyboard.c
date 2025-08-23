#include "keyboard.h"

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

    // Send reports
    if (keyboard_report_index > 0) {
        tud_hid_report(REPORT_ID_KEYBOARD, keyboard_report, sizeof(keyboard_report));
        clear_keyboard = true;
    } else if (clear_keyboard) {
        tud_hid_report(REPORT_ID_KEYBOARD, &(uint8_t){0}, sizeof(uint8_t));
        clear_keyboard = false;
    }

    if (consumer_report_index > 0) {
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &consumer_report[0], sizeof(consumer_report[0]));
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &(uint8_t){0}, sizeof(uint8_t));
    }

    return (keyboard_report_index > 0 || consumer_report_index > 0);
}
