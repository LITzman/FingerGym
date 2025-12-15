#ifndef KEYMAP_H
#define KEYMAP_H

/*
Everything regarding the order of keys
*/

static const uint8_t used_consumer_keys[] = {
    HID_USAGE_CONSUMER_PLAY_PAUSE,
    HID_USAGE_CONSUMER_VOLUME_INCREMENT,
    HID_USAGE_CONSUMER_VOLUME_DECREMENT,
};

struct key {
    const uint8_t key;

    // Aren't used, but comfortable for me
    const uint8_t row;
    const uint8_t col;
};

static const struct key keyboard_layout[MATRIX_ROW_COUNT][MATRIX_COL_COUNT] = {
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
        {HID_KEY_NONE, 0, 6},
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
        {HID_KEY_BACKSPACE, 1, 14},
        {HID_KEY_INSERT, 1, 15},
        {HID_KEY_HOME, 1, 16},
        {HID_KEY_PAGE_UP, 1, 17},
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
        {HID_KEY_DELETE, 2, 15},
        {HID_KEY_END, 2, 16},
        {HID_KEY_PAGE_DOWN, 2, 17},
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
        {HID_KEY_NONE, 3, 14},
        {HID_KEY_NONE, 3, 15},
        {HID_KEY_NONE, 3, 16},
        {HID_KEY_NONE, 3, 17},
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
        {HID_KEY_ARROW_UP, 4, 16},
        {HID_KEY_NONE, 4, 17},
    },
    {
        {HID_KEY_CONTROL_LEFT, 5, 0},
        {HID_KEY_GUI_LEFT, 5, 1},
        {HID_KEY_NONE, 5, 2},
        {HID_KEY_ALT_LEFT, 5, 3},
        {HID_KEY_NONE, 5, 4},
        {HID_KEY_NONE, 5, 5},
        {HID_KEY_SPACE, 5, 6},
        {HID_KEY_NONE, 5, 7},
        {HID_KEY_NONE, 5, 8},
        {HID_KEY_NONE, 5, 9},
        {HID_KEY_ALT_RIGHT, 5, 10},
        {HID_KEY_GUI_RIGHT, 5, 11},
        {HID_KEY_NONE, 5, 12},
        {HID_KEY_MENU, 5, 13}, // TODO, doesn't work
        {HID_KEY_CONTROL_RIGHT, 5, 14},
        {HID_KEY_ARROW_LEFT, 5, 15},
        {HID_KEY_ARROW_DOWN, 5, 16},
        {HID_KEY_ARROW_RIGHT, 5, 17},
    },
};


#endif // KEYMAP_H