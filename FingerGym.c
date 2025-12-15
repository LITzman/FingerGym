#include "tusb.h"
#include "keyboard.h"


void set_led_state(void) {
    // Measure time
    static uint32_t start_ms = 0;
    static bool led_state = false;

    if (led_interval == 0) {
        return;
    }

    if (start_ms == 0) {
        start_ms = board_millis();
    }

    uint32_t now = board_millis();
    if (now - start_ms >= led_interval) {
        led_state = !led_state;
        board_led_write(led_state);

        start_ms += led_interval * ((now - start_ms) / led_interval);
    }
}

void keyboard_task(void) {
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < POLLING_INTERVAL_MS) {
        return;
    }

    start_ms += POLLING_INTERVAL_MS;

    if (keyboard_update() && tud_suspended()) {
        // Get up
        if (remote_wakeup_enabled) {
            debug_print("Key press while suspended: attempting remote wakeup\n");
            tud_remote_wakeup();
        } else {
            debug_print("Key press while suspended but host didn't allow remote wakeup\n");
        }
    }
}

int main() {
    board_init();
    tusb_init();
    keyboard_init();

    while (true) {
        tud_task();
        set_led_state();
        keyboard_task();
    }

    return 0;
}