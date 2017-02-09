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
  /*
   *                   ,-----.                                                   ,-----.
   *             ,-----+  E  +-----------.                           ,-----------|  I  |-----.
   *       ,-----|  W  |-----|  R  |  T  |                           |  Y  |  U  |-----|  O  |-----.
   * ,-----|  A  |-----|  D  |-----+-----|                           |-----+-----|  K  |-----|  P  |-----.
   * |  Q  |-----|  S  |-----|  F  |  G  |                           |  H  |  J  |-----|  L  |-----| . > |
   * `-----|  Z  |-----|  C  |-----+-----'                           `-----+-----|  N  |-----| , < |-----'
   *       `-----|  X  |-----|  V  |       ,-------.       ,-------.       |  B  |-----|  M  |-----'
   *             `-----'     `-----'       |       |       |       |       `-----'     `-----'
   *                                       |  Fn1  |       |  Tab  |
   *                            ,-------.  | Lock  |       |       |  ,-------.
   *                            |S Ctl A|  `-------'       `-------'  |E AGr B|
   *                            |f     l|                             |n     s|
   *                            |t Fn0 t|                             |t SPC p|
   *                            `-------'                             `-------'
   */
  [0] = KEYMAP
  (
                         Key_E                                                   ,Key_I
                 ,Key_W        ,Key_R ,Key_T                       ,Key_Y ,Key_U        ,Key_O
          ,Key_A        ,Key_D                                                   ,Key_K        ,Key_P
   ,Key_Q        ,Key_S        ,Key_F ,Key_G                       ,Key_H ,Key_J        ,Key_L            ,Key_Period
          ,Key_Z        ,Key_C                                                   ,Key_N        ,Key_Comma
                 ,Key_X        ,Key_V                                     ,Key_B        ,Key_M

                        ,Key_Keymap1                                ,Key_Tab
           ,Key_Keymap1              ,Key_Keymap1          ,Key_Tab          ,Key_Tab
                        ,Key_Keymap1                                ,Key_Tab

                 ,Key_LCtrl                                               ,Key_RAlt
     ,Key_LShift               ,Key_LAlt                       ,Key_Enter           ,Key_Backspace
                 ,Key_Keymap0_Momentary                                   ,Key_Space
  )
};

void setup () {
  Serial.begin(9600);
  Keyboardio.setup (KEYMAP_SIZE);
}

void loop () {
  Keyboardio.loop ();
}
