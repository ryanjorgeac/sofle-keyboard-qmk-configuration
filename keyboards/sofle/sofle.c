// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include "quantum.h"
#include <avr/pgmspace.h>
// #include "keymaps/default/sofle_font.c"

#ifdef SWAP_HANDS_ENABLE
__attribute__ ((weak))
const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] =
    // The LAYOUT macro could work for this, but it was harder to figure out the
    // identity using it.

    // This is the identity layout.
/*
{
    { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0} },
    { {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1} },
    { {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2} },
    { {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3} },
    { {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4} },

    { {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5} },
    { {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6} },
    { {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7} },
    { {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8} },
    { {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9} },
};
*/

    // This is the mirror, q <-> p, w <-> o, etc...
{
 { {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5} },
 { {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6} },
 { {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7} },
 { {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8} },
 { {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9} },

 { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0} },
 { {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1} },
 { {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2} },
 { {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3} },
 { {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4} },
};

#    ifdef ENCODER_MAP_ENABLE
const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = {1, 0};
#    endif

#endif

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    oled_set_cursor(0, 0);
    return rotation;
}

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };
    oled_write_P(qmk_logo, false);
}

static void render_logo_tux(void) {
    static const char PROGMEM qmk_logo_tux[] = {
        153,154,10,
        185,186,0
    };

    oled_write_P(qmk_logo_tux, false);
}

static const char PROGMEM layer_box_top[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x00};
static const char PROGMEM layer_box_middle[] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0x00};
static const char PROGMEM layer_box_bottom[] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0x00};

static const char PROGMEM layer_qwerty = 0xA5;
static const char PROGMEM layer_symbols = 0xA6;
static const char PROGMEM layer_shortcuts = 0xA7;
static const char PROGMEM layer_unknown = 0x3F;

void print_status_narrow(void) {
    oled_clear();
    uint8_t layer = get_highest_layer(layer_state);

    oled_set_cursor(0, 0);

    char layer_middle_copy[sizeof(layer_box_middle)];
    memcpy_P(layer_middle_copy, layer_box_middle, sizeof(layer_box_middle));

    switch (layer) {
        case 0:
            layer_middle_copy[2] = layer_qwerty;
            break;
        case 1:
            layer_middle_copy[2] = layer_symbols;
            break;
        case 2:
            layer_middle_copy[2] = layer_shortcuts;
            break;
        default:
            layer_middle_copy[2] = layer_unknown;
            break;
    }

    oled_write_P(layer_box_top, false);
    oled_set_cursor(0, 1);
    oled_write(layer_middle_copy, false);
    oled_set_cursor(0, 2);
    oled_write_P(layer_box_bottom, false);

    oled_set_cursor(0, 3);

    switch (layer) {
        case 0:
            oled_write_P(PSTR("\nQWERT"), false);
            break;
        case 1:
            oled_write_P(PSTR("\nSYMBL"), false);
            break;
        case 2:
            oled_write_P(PSTR("\nSHCUT"), false);
            break;
        default:
            oled_write_P(PSTR("\n ??? "), false);
            break;
    }
    
    oled_write_P(PSTR("\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    if (led_usb_state.caps_lock) {
        oled_write_P(PSTR("CPSLK"), true);
    } else {
        oled_write_P(PSTR("CPSLK"), false);
    }
    
    oled_write_P(PSTR("\n\n"), false);
    render_logo_tux();
}

bool oled_task_kb(void) {
    if (!oled_task_user()) { return false; }
    
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }
    
    return false;
}

#endif

#ifdef ENCODER_ENABLE
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return true;
}
#endif