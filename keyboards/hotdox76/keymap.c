// Copyright 2021 JasonRen(biu)
// Copyright 2022 Drashna Jael're (@Drashna Jael're)
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum custom_keycodes {
  MT_LPRN = SAFE_RANGE,
  MT_RPRN,
  MT_AMPR,
};

#define CTRL_RPRN MT(MOD_LCTL, MT_RPRN)
#define ALT_LPRN MT(MOD_LALT, MT_LPRN)
#define GUI_AMPR MT(MOD_LGUI, MT_AMPR)
#define HOME_D MT(MOD_LCTL, KC_D)
#define HOME_S MT(MOD_LALT, KC_S)
#define HOME_J MT(MOD_RSFT, KC_J)
#define HOME_K MT(MOD_RCTL, KC_K)
#define HOME_L MT(MOD_RALT, KC_L)
#define HOME_SCLN MT(MOD_RGUI, KC_SCLN)
#define HOME_A MT(MOD_LGUI, KC_A)
#define HOME_F MT(MOD_LSFT, KC_F)

const uint16_t PROGMEM left_bracket_combo[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM right_bracket_combo[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM left_brace_combo[] = {HOME_F, HOME_D, COMBO_END};
const uint16_t PROGMEM right_brace_combo[] = {HOME_J, HOME_K, COMBO_END};

combo_t key_combos[] = {
    COMBO(left_bracket_combo, KC_LBRC), COMBO(right_bracket_combo, KC_RBRC),
    COMBO(left_brace_combo, KC_LCBR), COMBO(right_brace_combo, KC_RCBR)};

const key_override_t delete_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {
    &delete_key_override,
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case MT(MOD_LSFT, KC_F):
    return 150;
  /* case LT(1, KC_GRV):
      return 100; */
  case KC_J:
    return 50;
  default:
    return g_tapping_term;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case CTRL_RPRN:
    if (record->event.pressed && record->tap.count > 0) {
      tap_code16(KC_RPRN);
      return false;
    }
    break;
  case ALT_LPRN:
    if (record->event.pressed && record->tap.count > 0) {
      tap_code16(KC_LPRN);
      return false;
    }
    break;
  case GUI_AMPR:
    if (record->event.pressed && record->tap.count > 0) {
      tap_code16(KC_AMPR);
      return false;
    }
    break;
  default:
    return true; // Process all other keycodes normally
  }
  return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
     * │ `    │ 1 │ 2 │ 3 │ 4 │ 5 │ = │                     │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │    - │
     * ├──────┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──────┤
     * │ Tab  │ Q │ W │ E │ R │ T │ = │                     │ Y │ Y │ U │ I │ O │ P │    \ │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │Escape│ A │ S │ D │ F │ G ├───┤                     ├───┤ H │ J │ K │ L │ ; │    ' │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │Shift │ Z │ X │ C │ V │ B │ B │                     │ N │ N │ M │ , │ . │ / │ Shift│
     * └──┬───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼───┬──┘
     *    │Ctl│F4 │F5 │GUI│Alt│         │ C │ V │ │Alt│ A │         │ ← │ ↓ │ ↑ │ → │GUI│
     *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
     *                              │   │   │PgU│ │PgD│   │   │
     *                              │Bsp│Bsp├───┤ ├───┤Ent│   │
     *                              │   │   │Del│ │Ctl│   │   │
     *                              └───┴───┴───┘ └───┴───┴───┘
     */
    [0] = LAYOUT_ergodox_pretty(
        KC_MINS,        KC_1,         KC_2,    KC_3,   KC_4,          KC_5,           KC_LEFT,    KC_RIGHT,     KC_6,     KC_7,    KC_8,    KC_9,      KC_0,    KC_EQUAL,
        LGUI_T(KC_TAB), KC_Q,         KC_W,    KC_E,   KC_R,          KC_T,           _______,    _______,      KC_Y,     KC_U,    KC_I,    KC_O,      KC_P,    KC_MINUS,
                        KC_LEFT_CTRL, HOME_A,  HOME_S, HOME_D,        HOME_F,         LT(3,KC_G), KC_H,         HOME_J,   HOME_K,  HOME_L,  HOME_SCLN, KC_QUOT,
        OSM(MOD_LSFT),  KC_Z,         KC_X,    KC_C,   KC_V,          KC_B,           KC_B,       TG(1),        KC_N,     KC_M,    KC_COMM, KC_DOT,    KC_SLSH, OSM(MOD_RSFT),
                                      KC_LCTL, KC_F4,  KC_LGUI,       MO(2),          MO(1),      LT(1,KC_ESC), KC_DOWN,  KC_LBRC, KC_RBRC, KC_RGUI,
                                                                      KC_C,           KC_V,       KC_LEFT,      KC_RIGHT,
                                                                                      KC_PGUP,    KC_F1,
                                                       LT(3, KC_ENT), LGUI_T(KC_TAB), KC_DEL,     KC_ENT,       KC_BSPC,  KC_SPC
    ),
    [1] = LAYOUT_ergodox_pretty(
        KC_MINUS, LSFT(KC_EQUAL), LSFT(KC_COMM), LSFT(KC_DOT),  KC_F4,        KC_F5,                 KC_LEFT,       KC_RIGHT, KC_F6,              KC_F7,             KC_F8,             KC_F9,   KC_F10,  KC_MINS,
        KC_LGUI,  LSFT(KC_BSLS),  KC_LBRC,       KC_RBRC,       KC_EQUAL,     KC_QUOT,               _______,       _______,  RSFT(KC_QUOT),      KC_7,              KC_8,              KC_9,    KC_BSLS, _______,
                  KC_LCTL,        GUI_AMPR,      ALT_LPRN,      CTRL_RPRN,    MT(MOD_LSFT, KC_SCLN), LSFT(KC_SCLN), KC_MINUS, MT(MOD_RSFT, KC_4), MT(MOD_RCTL,KC_5), MT(MOD_RALT,KC_6), _______, KC_ENT,
        KC_ESC,   LSFT(KC_1),     LSFT(KC_LBRC), LSFT(KC_RBRC), LSFT(KC_GRV), LSFT(KC_MINS),         _______,       TG(1),    KC_GRV,             KC_1,              KC_2,              KC_3,    _______, KC_RSFT,
                                  KC_LCTL,       KC_F4,         KC_F5,        _______,               _______,       KC_0,     KC_0,               KC_DOT,            KC_DOT,            KC_RGUI,
                                                                              KC_C,                  KC_V,          KC_RALT,  KC_A,
                                                                                                     KC_PGUP,       _______,
                                                                _______,      _______,               KC_DEL,        KC_RCTL,  KC_ENT,             KC_SPC
    ),
    [2] = LAYOUT_ergodox_pretty(
        KC_F1,     KC_MUTE, KC_VOLD, KC_VOLU, _______,  _______, _______, _______, _______, _______,   _______,       _______, _______, QK_BOOTLOADER,
        LAG(KC_N), _______, KC_UP,   KC_ENT,  _______,  _______, _______, _______, _______, _______,   _______,       _______, _______, _______,
                   _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______, KC_LEFT, KC_DOWN, KC_UP,     KC_RIGHT,      _______, _______,
        _______,   _______, _______, _______, _______,  _______, _______, _______, _______, LAG(KC_W), LGUI(KC_LBRC), _______, _______, _______,
                            _______, _______, _______,  _______, _______, _______, _______, _______,   _______,       _______,
                                                        _______, _______, _______, _______,
                                                                 _______, _______,
                                              _______,  _______, _______, _______, _______, _______
    ),
    [3] = LAYOUT_ergodox_pretty(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_AMPR, KC_ASTR, KC_LPRN, _______, _______,
                 _______, _______, _______, _______, _______, _______, KC_PLUS, KC_DLR,  KC_PERC, KC_CIRC, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_EXLM, KC_AT,   KC_HASH, _______, _______,
                          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                                     _______, _______, _______, _______,
                                                              _______, _______,
                                            _______, _______, _______, _______, _______, _______
    )
};

