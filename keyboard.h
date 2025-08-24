#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "hardware/gpio.h"
#include "class/hid/hid.h"
#include "usb_descriptors.h"
#include "tusb.h"
#include "wiring.h"
#include "keymap.h"

#define MAX_REPORT_KEYS 6 // Defined by tud_hid_keyboard_report,
#define MAX_REPORT_CONSUMER 1 // I don't need any more

enum {
    BLINK_CAPS_LOCK = 0,
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

extern uint32_t led_interval;

void keyboard_init(void);
bool keyboard_update(void);

#endif // KEYBOARD_H