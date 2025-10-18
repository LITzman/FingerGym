#include "keyboard.h"
#include <stdarg.h>
#include <stdio.h>

static uint8_t last_keyboard_report[KEYBOARD_REPORT_SIZE] = {0};
static uint8_t last_consumer_report[MAX_REPORT_CONSUMER] = {0};
static uint8_t encoder_last_state = 0;
uint32_t led_interval = BLINK_NOT_MOUNTED;

static const int8_t encoder_states[16] = {
    0, 1, -1, 0,
    -1, 0, 0, 1,
    1, 0, 0, -1,
    0 -1, 1, 0,
};

void debug_print(const char* fmt, ...) {
    if (!tud_cdc_connected()) return;

    char buf[128]; // adjust size as needed
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    tud_cdc_write_str(buf);
    tud_cdc_write_flush();
}

void keyboard_init(void) {

    // Initialize matrix column pins - output pins
    for (int i = 0; i < MATRIX_COL_COUNT; i++) {
        gpio_init(matrix_col_pins[i]);
        gpio_set_dir(matrix_col_pins[i], GPIO_OUT);
        // gpio_set_dir(matrix_col_pins[i], GPIO_IN);
        gpio_put(matrix_col_pins[i], false);
        // gpio_disable_pulls(matrix_col_pins[i]);
    }

    // Initialize matrix row pins - input pins
    for (int i = 0; i < MATRIX_ROW_COUNT; i++) {
        gpio_init(matrix_row_pins[i]);
        gpio_set_dir(matrix_row_pins[i], GPIO_IN);
        gpio_pull_down(matrix_row_pins[i]);
        // gpio_pull_down(matrix_row_pins[i]);
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

    uint8_t keyboard_report[KEYBOARD_REPORT_SIZE] = {0};
    uint8_t consumer_report[MAX_REPORT_CONSUMER] = {0}; // Ah

    uint8_t keyboard_report_index = 0;
    uint8_t consumer_report_index = 0;

    uint8_t modifier = 0;

    static int8_t encoder_accumulator = 0;

    // Poll encoder changes
    uint8_t encoder_current_state = (gpio_get(ENCODER_A_PIN) << 1) | gpio_get(ENCODER_B_PIN);

    // Hail GPT
    int8_t encoder_direction = encoder_states[(encoder_last_state << 2) | encoder_current_state];
    encoder_last_state = encoder_current_state;

    if (encoder_direction != 0) {
        encoder_accumulator += encoder_direction;
    }

    if (encoder_current_state == 0b00 && encoder_accumulator != 0) {
        if (encoder_accumulator > 0) {
            // debug_print("Encoder Increase\n");
            if (consumer_report_index < MAX_REPORT_CONSUMER) {
                consumer_report[consumer_report_index++] = HID_USAGE_CONSUMER_VOLUME_INCREMENT;
            } 
        } else {
            // debug_print("Encoder Decrease\n");
            if (consumer_report_index < MAX_REPORT_CONSUMER) {
                consumer_report[consumer_report_index++] = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
            }
        }
    }

    encoder_accumulator = 0;

    // Test each row
    for (int col = 0; col < MATRIX_COL_COUNT; col++) {

        for (int i = 0; i < MATRIX_COL_COUNT; i++) {
            gpio_set_dir(matrix_col_pins[i], GPIO_OUT);
            gpio_disable_pulls(matrix_col_pins[i]); // float
        }

        // Activate row
        gpio_put(matrix_col_pins[col], true);

        sleep_us(30);

        for (int row = 0; row < MATRIX_ROW_COUNT; row++) {

            // Test col
            if (!gpio_get(matrix_row_pins[row])) {
                continue;
            }

            uint8_t key = keyboard_layout[row][col].key;

            if (key == HID_KEY_NONE) {
                continue;
            }

            // Activate modifiers
            switch (key) {
                case HID_KEY_SHIFT_LEFT: modifier |= 0x02; continue;
                case HID_KEY_SHIFT_RIGHT: modifier |= 0x20; continue;
                case HID_KEY_CONTROL_LEFT: modifier |= 0x01; continue;
                case HID_KEY_CONTROL_RIGHT: modifier |= 0x10; continue;
                case HID_KEY_ALT_LEFT: modifier |= 0x04; continue;
                case HID_KEY_ALT_RIGHT: modifier |= 0x40; continue;
                case HID_KEY_GUI_LEFT: modifier |= 0x08; continue;
                case HID_KEY_GUI_RIGHT: modifier |= 0x80; continue;
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
                // debug_print("Key pressed: row=%d col=%d\n", row, col);
                // debug_print("Modifier byte: 0x%02X\n", modifier);
                keyboard_report[2 + keyboard_report_index] = key;
                keyboard_report_index++;
            }
        }

        // Chill row
        gpio_put(matrix_col_pins[col], false);
    }

    keyboard_report[0] = modifier;
    keyboard_report[1] = 0;

    // Send reports
    if (memcmp(last_keyboard_report, keyboard_report, KEYBOARD_REPORT_SIZE) != 0) {
        tud_hid_report(REPORT_ID_KEYBOARD, keyboard_report, sizeof(keyboard_report));
        memcpy(last_keyboard_report, keyboard_report, KEYBOARD_REPORT_SIZE);
    }

    if (memcmp(last_consumer_report, consumer_report, MAX_REPORT_CONSUMER) != 0) {
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, consumer_report, MAX_REPORT_CONSUMER);
        memcpy(last_consumer_report, consumer_report, MAX_REPORT_CONSUMER);
    }

    // Return true if any key was pressed
    return (keyboard_report_index > 0 || consumer_report_index > 0);
}
