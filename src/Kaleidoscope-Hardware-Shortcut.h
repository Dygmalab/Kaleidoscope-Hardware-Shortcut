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

#pragma once

#include <Arduino.h>
#include <AtmegaScanner.h>

#define HARDWARE_IMPLEMENTATION Shortcut

#define COLS 14
#define ROWS 4

typedef struct {
  uint8_t g;
  uint8_t r;
  uint8_t b;
} cRGB;

#define CRGB(r, g, b) (cRGB){g, r, b}

#define LED_COUNT 16

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

    AtmegaScanner<COLS, ROWS> scanner;

  private:
    cRGB leds[LED_COUNT];
};

#define KEYMAP(                                                                                 \
                     r0c2                                               ,r0cb                   \
              ,r0c1       ,r0c3 ,r0c4                       ,r0c9 ,r0ca       ,r0cc             \
        ,r0c0       ,r1c2                                               ,r1cb       ,r0cd       \
  ,r2c0       ,r1c1       ,r1c3 ,r1c4                       ,r1c9 ,r1ca       ,r1cc       ,r2cd \
        ,r1c0       ,r2c2                                               ,r2cb       ,r1cd       \
              ,r2c1       ,r2c3                                   ,r2ca       ,r2cc             \
                                                                                                \
                                 ,r0c6                     ,r0c7                                \
                           ,r1c6       ,r3c6         ,r1c7       ,r3c7                          \
                                 ,r2c6                     ,r2c7                                \
                                                                                                \
                          ,r2c5                                  ,r2c8                          \
                    ,r3c5        ,r1c5                     ,r3c8       ,r1c8                    \
                          ,r0c5                                  ,r0c8                          \
 )                                                                                              \
 {                                                                                              \
   {r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0ca, r0cb, r0cc, r0cd},        \
   {r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1ca, r1cb, r1cc, r1cd},        \
   {r2c0, r2c1, r2c2, r2c3,  XXX, r2c5, r2c6, r2c7, r2c8,  XXX, r2ca, r2cb, r2cc, r2cd},        \
   { XXX,  XXX,  XXX,  XXX,  XXX, r3c5, r3c6, r3c7, r3c8,  XXX,  XXX,  XXX,  XXX,  XXX}         \
 }

#define SCANBIT(row,col) ((uint32_t)1 << (row * (COLS / 2) + col))

#define R0C0  SCANBIT(0, 0)
#define R0C1  SCANBIT(0, 1)
#define R0C2  SCANBIT(0, 2)
#define R0C3  SCANBIT(0, 3)
#define R0C4  SCANBIT(0, 4)
#define R0C5  SCANBIT(0, 5)
#define R0C6  SCANBIT(0, 6)
#define R1C0  SCANBIT(1, 0)
#define R1C1  SCANBIT(1, 1)
#define R1C2  SCANBIT(1, 2)
#define R1C3  SCANBIT(1, 3)
#define R1C4  SCANBIT(1, 4)
#define R1C5  SCANBIT(1, 5)
#define R1C6  SCANBIT(1, 6)
#define R2C0  SCANBIT(2, 0)
#define R2C1  SCANBIT(2, 1)
#define R2C2  SCANBIT(2, 2)
#define R2C3  SCANBIT(2, 3)
#define R2C4  SCANBIT(2, 4)
#define R2C5  SCANBIT(2, 5)
#define R2C6  SCANBIT(2, 6)
#define R3C0  SCANBIT(3, 0)
#define R3C1  SCANBIT(3, 1)
#define R3C2  SCANBIT(3, 2)
#define R3C3  SCANBIT(3, 3)
#define R3C4  SCANBIT(3, 4)
#define R3C5  SCANBIT(3, 5)
#define R3C6  SCANBIT(3, 6)

#define R0C7   SCANBIT(0, 6)
#define R0C8   SCANBIT(0, 5)
#define R0C9   SCANBIT(0, 4)
#define R0C10  SCANBIT(0, 3)
#define R0C11  SCANBIT(0, 2)
#define R0C12  SCANBIT(0, 1)
#define R0C13  SCANBIT(0, 0)
#define R1C7   SCANBIT(1, 6)
#define R1C8   SCANBIT(1, 5)
#define R1C9   SCANBIT(1, 4)
#define R1C10  SCANBIT(1, 3)
#define R1C11  SCANBIT(1, 2)
#define R1C12  SCANBIT(1, 1)
#define R1C13  SCANBIT(1, 0)
#define R2C7   SCANBIT(2, 6)
#define R2C8   SCANBIT(2, 5)
#define R2C9   SCANBIT(2, 4)
#define R2C10  SCANBIT(2, 3)
#define R2C11  SCANBIT(2, 2)
#define R2C12  SCANBIT(2, 1)
#define R2C13  SCANBIT(2, 0)
#define R3C7   SCANBIT(3, 6)
#define R3C8   SCANBIT(3, 5)
#define R3C9   SCANBIT(3, 4)
#define R3C10  SCANBIT(3, 3)
#define R3C11  SCANBIT(3, 2)
#define R3C12  SCANBIT(3, 1)
#define R3C13  SCANBIT(3, 0)
