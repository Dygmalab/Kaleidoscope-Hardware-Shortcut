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

void Shortcut::unselectRows(void) {
    DDRD &= ~0b00110000;
    PORTD &= ~0b00110000;

    DDRE &= ~0b01000100;
    PORTE &= ~0b01000100;
}

void Shortcut::initCols(void) {
    DDRB &= ~(1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7);
    PORTB |= (1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7);
    DDRC &= ~(1<<7 | 1<<6);
    PORTC |= (1<<7 | 1<<6);
    DDRD &= ~(1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<7);
    PORTD |= (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<7);
    DDRF &= ~(1<<6 | 1<<7);
    PORTF |= (1<<6 | 1<<7);
}

void Shortcut::selectRow(uint8_t row)
{
    switch (row) {
    case 0:
        DDRD  |= (1<<4);
        PORTD &= ~(1<<4);
        break;
    case 1:
        DDRD  |= (1<<5);
        PORTD &= ~(1<<5);
        break;
    case 2:
        DDRE  |= (1<<2);
        PORTE &= ~(1<<2);
        break;
    case 3:
        DDRE  |= (1<<6);
        PORTE &= ~(1<<6);
        break;
    default:
        break;
    }
}

uint16_t Shortcut::readCols(void)
{
    return (PINF&(1<<6) ? 0 : (1<<0)) |
           (PINF&(1<<7) ? 0 : (1<<1)) |
           (PINB&(1<<4) ? 0 : (1<<2)) |
           (PINB&(1<<5) ? 0 : (1<<3)) |
           (PINB&(1<<6) ? 0 : (1<<4)) |
           (PIND&(1<<7) ? 0 : (1<<5)) |
           (PINC&(1<<6) ? 0 : (1<<6)) |
           (PINC&(1<<7) ? 0 : (1<<7)) |
           (PIND&(1<<3) ? 0 : (1<<8)) |     // Rev.A and B
           (PIND&(1<<2) ? 0 : (1<<9)) |
           (PIND&(1<<1) ? 0 : (1<<10)) |
           (PIND&(1<<0) ? 0 : (1<<11)) |
           (PINB&(1<<7) ? 0 : (1<<12)) |
           (PINB&(1<<3) ? 0 : (1<<13));
}

void Shortcut::setup(void) {
    leftHandState.all = 0;
    rightHandState.all = 0;

    unselectRows();
    initCols();
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
    previousLeftHandState = leftHandState;
    previousRightHandState = rightHandState;

    for (uint8_t i = 0; i < ROWS; i++) {
        selectRow(i);
        uint16_t cols = readCols();

        for (uint8_t c = 0; c < COLS / 2; c++) {
            uint8_t pos = i * (COLS / 2) + c;
            bitWrite(leftHandState.all, pos, bitRead(cols, c));
            bitWrite(rightHandState.all, pos, bitRead(cols, c + (COLS / 2)));

#if 0
            if (bitRead(cols, c)) {
                Serial.print ("(");
                Serial.print (i);
                Serial.print (", ");
                Serial.print (c);
                Serial.println (")");
            }
            if (bitRead(cols, c + (COLS / 2))) {
                Serial.print ("(");
                Serial.print (i);
                Serial.print (", ");
                Serial.print (c + (COLS / 2));
                Serial.println (")");
            }
#endif
        }

       unselectRows();
    }
}

void Shortcut::act_on_matrix_scan() {
    for (byte row = 0; row < ROWS; row++) {
        for (byte col = 0; col < COLS / 2; col++) {
            uint8_t keynum = (row * (COLS / 2)) + col;

            uint8_t keyState = (bitRead(previousLeftHandState.all, keynum) << 0) |
              (bitRead(leftHandState.all, keynum) << 1);
            handle_key_event(Key_NoKey, row, col, keyState);

            keyState = (bitRead(previousRightHandState.all, keynum) << 0) |
              (bitRead(rightHandState.all, keynum) << 1);

            handle_key_event(Key_NoKey, row, col, keyState);
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
