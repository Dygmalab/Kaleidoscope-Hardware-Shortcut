/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Shortcut -- Shortcut hardware support for KaleidoscopeFirmware
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

#include <Kaleidoscope.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include "Light_WS2812/light_ws2812.h"

uint32_t Shortcut::leftHandMask;
uint32_t Shortcut::rightHandMask;

Shortcut::Shortcut(void) {
}

void Shortcut::setup(void) {
  wdt_disable();
  delay(100);

  for (int i = 0; i < LED_COUNT; i++)
    setCrgbAt(i, CRGB(0, 0, 0));
  syncLeds();
}

void Shortcut::setCrgbAt(uint8_t i, cRGB crgb) {
    leds[i] = crgb;
}

void Shortcut::setCrgbAt(byte row, byte col, cRGB color) {
}

cRGB Shortcut::getCrgbAt(uint8_t i) {
    return leds[i];
}

void Shortcut::syncLeds() {
    ws2812_sendarray((uint8_t *)leds,sizeof(cRGB) * LED_COUNT);
}

void Shortcut::readMatrix() {
    scanner.readKeys();
}

void Shortcut::actOnMatrixScan() {
    for (byte row = 0; row < ROWS; row++) {
        for (byte col = 0; col < COLS / 2; col++) {
            uint8_t keynum = (row * (COLS / 2)) + col;

            uint8_t keyState = (bitRead(scanner.previousLeftHandState.all, keynum) << 0) |
              (bitRead(scanner.leftHandState.all, keynum) << 1);
            handleKeyswitchEvent(Key_NoKey, row, col, keyState);

            keyState = (bitRead(scanner.previousRightHandState.all, keynum) << 0) |
              (bitRead(scanner.rightHandState.all, keynum) << 1);
            handleKeyswitchEvent(Key_NoKey, row, COLS / 2 + col, keyState);
        }
    }
}

void Shortcut::scanMatrix() {
    readMatrix();
    actOnMatrixScan();
}

void Shortcut::maskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  if (col >= 8) {
    rightHandMask |= SCANBIT(row, col - 8);
  } else {
    leftHandMask |= SCANBIT(row, col);
  }
}

void Shortcut::unMaskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  if (col >= 8) {
    rightHandMask &= ~(SCANBIT(row, col - 8));
  } else {
    leftHandMask &= ~(SCANBIT(row, col));
  }
}

bool Shortcut::isKeyMasked(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return false;

  if (col >= 8) {
    return rightHandMask & SCANBIT(row, col - 8);
  } else {
    return leftHandMask & SCANBIT(row, col);
  }
}

void Shortcut::maskHeldKeys(void) {
  rightHandMask = scanner.rightHandState.all;
  leftHandMask = scanner.leftHandState.all;
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
