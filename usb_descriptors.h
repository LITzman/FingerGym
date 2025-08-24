#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include "bsp/board_api.h"
#include "tusb.h"

/*
Constants for all sorts of USB setups and reports.
Documented only in nordic ruins
*/

enum {
    REPORT_ID_KEYBOARD = 1,
    REPORT_ID_CONSUMER_CONTROL,
};

static uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL)),
};

static tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,

    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = 0xCAFE,
    .idProduct = 0x4000,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01
};

#define EPNUM_CDC_NOTIF   0x82
#define EPNUM_CDC_OUT     0x02
#define EPNUM_CDC_IN      0x83

static uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, 3, 0, (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN + TUD_CDC_DESC_LEN),
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_NONE,
                       sizeof(desc_hid_report), 0x81, CFG_TUD_HID_EP_BUFSIZE, POLLING_INTERVAL_MS),

    TUD_CDC_DESCRIPTOR(1, 0, EPNUM_CDC_NOTIF, 8,
                       EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
};

static char const *string_desc_arr[] = {
        (const char[]){0x09, 0x04},
        "LITzman",
        "FingerGym",
        "WhatAreYouLookingAt",
};

#define _DESC_STR_SIZE 32
static uint16_t _desc_str[_DESC_STR_SIZE];

#endif // USB_DESCRIPTORS_H