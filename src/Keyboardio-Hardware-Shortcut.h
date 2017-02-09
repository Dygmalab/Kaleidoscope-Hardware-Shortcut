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

#pragma once

#include <Arduino.h>
#include <AtmegaScanner.h>

#define HARDWARE_IMPLEMENTATION Shortcut

#define COLS 14
#define ROWS 4

typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} cRGB;

class Shortcut {
  public:
    Shortcut(void);
    void led_sync(void);
    void led_set_crgb_at(byte row, byte col, cRGB color);
    void led_set_crgb_at(uint8_t i, cRGB crgb);
    cRGB led_get_crgb_at(uint8_t i);
    cRGB get_key_color(byte row, byte col);

    void scan_matrix(void);
    void read_matrix(void);
    void act_on_matrix_scan(void);
    void setup();

    uint8_t load_primary_layer(uint8_t layer_count);
    void save_primary_layer(uint8_t layer);

  private:
    AtmegaScanner<COLS, ROWS> scanner;
};

#define LED_COUNT 2

#define KEYMAP(                                                                                 \
                     r0c2                                               ,r0c9                   \
              ,r0c1       ,r0c3 ,r0c4                       ,r0cb ,r0ca       ,r0c8             \
        ,r0c0       ,r1c2                                               ,r1c9       ,r0c7       \
  ,r2c0       ,r1c1       ,r1c3 ,r1c4                       ,r1cb ,r1ca       ,r1c8       ,r2c7 \
        ,r1c0       ,r2c2                                               ,r2c9       ,r1c7       \
              ,r2c1       ,r2c3                                   ,r2ca       ,r2c8             \
                                                                                                \
                                 ,r2c5                     ,r2cc                                \
                           ,r3c5       ,r1c5         ,r1cc       ,r3cc                          \
                                 ,r0c5                     ,r0cc                                \
                                                                                                \
                          ,r0c6                                  ,r0cd                          \
                    ,r1c6        ,r3c6                     ,r3cd       ,r1cd                    \
                          ,r2c6                                  ,r2cd                          \
 )                                                                                              \
 {                                                                                              \
   {r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0ca, r0cb, r0cd},              \
   {r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1ca, r1cb, r1cd},              \
   {r2c0, r2c1, r2c2, r2c3,  XXX, r2c5, r2c6, r2c7, r2c8, r2c9, r2ca,  XXX, r2cd},              \
   { XXX,  XXX,  XXX,  XXX,  XXX, r3c5, r3c6,  XXX,  XXX,  XXX,  XXX,  XXX, r3cd}               \
 }
