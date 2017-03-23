#!/usr/bin/env python
## shortcut-remap -- Change the keymap on a Shortcut
## Copyright (C) 2017  Gergely Nagy
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

## This is a very, very crude hack at the moment. Please don't hurt me.

import sys
import re
import serial

class KeymapParser:
    __mapping = {
        'tab': 'Tab',
        'xxx': 'XXX',
        '---': '---',
        'spc': 'Space',
        'space': 'Space',
        'ent': 'Enter',
        'enter': 'Enter',
        'alt': 'LAlt',
        'agr': 'RAlt',
        'altgr': 'RAlt',
        'lalt': 'LAlt',
        'ralt': 'RAlt',
        'sft': 'LShift',
        'lsft': 'LShift',
        'rsft': 'RShift',
        'ctl': 'LControl',
        'ctrl': 'LControl',
        'control': 'LControl',
        'lctl': 'LControl',
        'lctrl': 'LControl',
        'lcontrol': 'LControl',
        'rctl': 'RControl',
        'rctrl': 'RControl',
        'rcontrol': 'RControl',
        'gui': 'LGUI',
        'lgui': 'LGUI',
        'rgui': 'RGUI',
        'bsp': 'Backspace',
        'backspace': 'Backspace',
        'del': 'Del',
        'esc': 'Esc',
    }

    __arrangement = [
        [(0, 2), (0, 0xb)],
        [(0, 1), (0, 3), (0, 4), (0, 9), (0, 0xa), (0, 0xc)],
        [(0, 0), (1, 2), (1, 0xb), (0, 0xd)],
        [(2, 0), (1, 1), (1, 3), (1, 4), (1, 9), (1, 0xa), (1, 0xc), (2, 0xd)],
        [(1, 0), (2, 2), (2, 0xb), (1, 0xd)],
        [(2, 1), (2, 3), (2, 0xa), (2, 0xc)],
        [(0, 6), (0, 7)],
        [(1, 6), (3, 6), (1, 7), (3, 7)],
        [(2, 6), (2, 7)],
        [(2, 5), (2, 8)],
        [(3, 5), (1, 5), (3, 8), (1, 8)],
        [(0, 5), (0, 8)]
    ]

    def parseLine (self, line):
        return [elem.strip() for elem in re.split ("\|\s*([^\|]+)\s*\|", line) if elem.strip() != '' and not re.match("[,`]?-----[\.,]?", elem.strip())]

    def parseLayer(self, f):
        layout=[]
        while True:
            line = f.readline ().strip ()
            if line.startswith ("#"):
                continue
            if line == "":
                break
            parsed = self.parseLine (line)
            if len(parsed) > 0:
                layout.append (parsed)
        return layout

    def normalizeKey(self, key):
        if (len(key) == 1) and ((key >= 'A' and key <= 'Z') or (key >= '0' and key <= '9')):
            return key.upper()

        if key.lower() in self.__mapping:
            return self.__mapping[key.lower()]

        return key

    def reArrangeLayer(self, a):
        flat = (14 * 4) * ['XXX']
        for line in range(len(a)):
            for col in range(len(a[line])):
                r, c = self.__arrangement[line][col]
                loc = r * 14 + c
                flat[loc] = a[line][col]
        return flat

    def parseInput(self, f):
        keymap = []
        while True:
            layer = self.parseLayer (f)
            if layer == []:
                break
            keymap.extend (map(lambda k: self.normalizeKey(k), self.reArrangeLayer(layer)))
        return keymap


class KeyCodeMapper:
    __keyCodes = {
        'XXX': 0,
        '---': 255 * 256 + 255,
        'Enter': 40,
        'Esc': 41,
        'Backspace': 42,
        'Tab': 43,
        'Space': 44,
    }
    def toCode(self, key):
        if (len(key) == 1):
            if (key >= 'A' and key <= 'Z'):
                return ord(key) - ord('A') + 4
            if (key >= '1' and key <= '9'):
                return ord(key) - ord('0') + 30
            if key == '0':
                return 39
        if key in self.__keyCodes:
            return self.__keyCodes[key]

        return 0

    def map(self, codes):
        return map (lambda k: str (self.toCode (k)), codes)

#### MAIN ####

parser = KeymapParser()
mapper = KeyCodeMapper()

keymap = parser.parseInput (sys.stdin)
codes = mapper.map (keymap)

if len(sys.argv) == 1:
    device = "/dev/ttyACM0"
else:
    device = sys.argv[1]

if device == '-':
    print "keymap.upload {0}\n".format (" ".join (codes)),
else:
    with serial.Serial (device, 9600, timeout = 1) as ser:
        ser.write ("keymap.upload {0}\n".format (" ".join (codes)))
