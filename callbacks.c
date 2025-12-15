#include "tusb.h"
#include "keyboard.h"

/*
Necessary callback implementations for tinyusb.
*/

uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    return desc_configuration;
}

uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
    return desc_hid_report;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {

    // Kind of funny to make a caps lock indicator that's under the keyboard
    if (report_type == HID_REPORT_TYPE_OUTPUT) {
        if (report_id == REPORT_ID_KEYBOARD) {

            if (bufsize < 1) {
                return;
            }

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
                debug_print("Caps Lock\n");
                led_interval = BLINK_CAPS_LOCK;
            } else{
                led_interval = BLINK_MOUNTED;
            }
        }
    }
}

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {

    if (index == 0) {
        _desc_str[0] = (TUSB_DESC_STRING << 8 ) | 0x02;
        _desc_str[1] = 0x0409; // English
        return _desc_str;
    }

    if (index >= sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) {
        return NULL;
    }

    const char* str = string_desc_arr[index];
    size_t chr_count = strlen(str);
    if (chr_count > (_DESC_STR_SIZE - 1)) {
        chr_count = _DESC_STR_SIZE - 1;
    }

    for (size_t i = 0; i < chr_count; i++) {
        _desc_str[1 + i] = str[i];
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

    return _desc_str;
}


// Lights show
void tud_mount_cb(void) {
    led_interval = BLINK_MOUNTED;
    keyboard_reset();
}

void tud_umount_cb(void) {
    led_interval = BLINK_NOT_MOUNTED;
    keyboard_reset();
}

void tud_suspend_cb(bool remote_wakeup_en) {
    led_interval = BLINK_SUSPENDED;
    keyboard_reset();
}

void tud_resume_cb(void) {
    led_interval = BLINK_MOUNTED;
    keyboard_reset();
}
