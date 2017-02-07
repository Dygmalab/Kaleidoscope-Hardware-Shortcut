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

#include "KeyboardioFirmware.h"

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED
  (
           Key_W,       Key_E,       Key_R,       Key_T,
           Key_Q,       Key_S,       Key_D,       Key_F,       Key_G,
     Key_A,
           Key_Z,       Key_X,       Key_C,       Key_V,
              Key_Keymap1,
           Key_NoKey, Key_NoKey,
              Key_NoKey,
              Key_NoKey,
           Key_NoKey, Key_NoKey,
              Key_NoKey,

           Key_U,       Key_I,       Key_O,       Key_P,
           Key_Y,       Key_H,       Key_J,       Key_K,       Key_L,
     Key_Enter,
           Key_B,       Key_N,       Key_M,       Key_NoKey,
              Key_NoKey,
           Key_NoKey, Key_NoKey,
              Key_NoKey,
              Key_NoKey,
           Key_NoKey, Key_NoKey,
              Key_NoKey
  ),
  [1] = KEYMAP_STACKED
  (
           Key_X,       Key_X,       Key_X,       Key_X,
           Key_X,       Key_X,       Key_X,       Key_X,       Key_X,
     Key_X,
           Key_X,       Key_X,       Key_X,       Key_X,
              Key_Keymap1,
           Key_X, Key_X,
              Key_X,
              Key_X,
           Key_X, Key_X,
              Key_X,

           Key_X,       Key_X,       Key_X,       Key_X,
           Key_X,       Key_X,       Key_X,       Key_X,       Key_X,
     Key_X,
           Key_X,       Key_X,       Key_X,       Key_X,
              Key_X,
           Key_X, Key_X,
              Key_X,
              Key_X,
           Key_X, Key_X,
              Key_X
  ),
};

void setup () {
  Serial.begin(9600);
  Keyboardio.setup (KEYMAP_SIZE);
}

void loop () {
  Keyboardio.loop ();
}
