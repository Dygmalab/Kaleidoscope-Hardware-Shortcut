/* -*- mode: c++ -*-
 * Keyboardio-Hardware-Shortcut -- Shortcut hardware support for KeyboardioFirmware
 * Copyright (C) 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <KeyboardioFirmware.h>
#include <avr/wdt.h>
#include <EEPROM.h>

#define EEPROM_LAYER_LOCATION 0

Shortcut::Shortcut(void) {
}

void Shortcut::setup(void) {
}

void Shortcut::led_set_crgb_at(uint8_t i, cRGB crgb) {
}

void Shortcut::led_set_crgb_at(byte row, byte col, cRGB color) {
}

cRGB Shortcut::led_get_crgb_at(uint8_t i) {
    return {0, 0, 0};
}

void Shortcut::led_sync() {
}

void Shortcut::read_matrix() {
    scanner.readKeys();
}

void Shortcut::act_on_matrix_scan() {
    for (byte row = 0; row < ROWS; row++) {
        for (byte col = 0; col < COLS / 2; col++) {
            uint8_t keynum = (row * (COLS / 2)) + col;

            uint8_t keyState = (bitRead(scanner.previousLeftHandState.all, keynum) << 0) |
              (bitRead(scanner.leftHandState.all, keynum) << 1);
            handle_key_event(Key_NoKey, row, col, keyState);

            keyState = (bitRead(scanner.previousRightHandState.all, keynum) << 0) |
              (bitRead(scanner.rightHandState.all, keynum) << 1);
            handle_key_event(Key_NoKey, row, COLS - col - 1, keyState);
        }
    }
}

void Shortcut::scan_matrix() {
    read_matrix();
    act_on_matrix_scan();
}

void Shortcut::save_primary_layer(uint8_t layer) {
    EEPROM.write(EEPROM_LAYER_LOCATION, layer);
}

uint8_t Shortcut::load_primary_layer(uint8_t layer_count) {
    uint8_t layer =  EEPROM.read(EEPROM_LAYER_LOCATION);
    if (layer >= layer_count) {
        return 0; // undefined positions get saved as 255
    }
    return 0; //  return keymap;
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
