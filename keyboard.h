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

static uint8_t keyboard_report[MAX_REPORT_KEYS];
static uint8_t consumer_report[MAX_REPORT_CONSUMER]; // Ah

static bool clear_keyboard = false;
static uint8_t encoder_last_state = 0;

enum {
    BLINK_CAPS_LOCK = 0,
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};
static uint32_t led_interval = BLINK_NOT_MOUNTED;

void keyboard_init(void);
bool keyboard_update(void);

#endif // KEYBOARD_H