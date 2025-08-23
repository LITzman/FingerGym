#include "tusb.h"
#include "keyboard.h"

void set_led_state(void) {
    // Measure time
    static uint32_t start_ms = 0;
    static bool led_state = false;

    if (start_ms == 0) {
        start_ms = board_millis();
    }

    uint32_t now = board_millis();
    if (now - start_ms >= led_interval) {
        led_state != led_state;
        board_led_write(led_state);

        start_ms += led_interval * ((now - start_ms) / led_interval);
    }
}

int main()
{
    board_init();
    tusb_init();
    keyboard_init();

    // Print something?

    while (true) {
        set_led_state();

    }
    return 0;
}