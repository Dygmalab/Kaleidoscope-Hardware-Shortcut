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
        'caps': 'CapsLock',
        'capsl': 'CapsLock',
        'capl': 'CapsLock',
        'cl': 'CapsLock',
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

        if key.lower().endswith ('*') and key.lower()[:-1] in self.__mapping:
            return self.__mapping[key.lower()[:-1]] + '*'

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
    __keyCodes = [
        None,
        None,
        None,
        None,
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P',
        'Q',
        'R',
        'S',
        'T',
        'U',
        'V',
        'W',
        'X',
        'Y',
        'Z',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        'Enter',
        'Esc',
        'Backspace',
        'Tab',
        'Space',
        '-',
        '=',
        '[',
        ']',
        '\\',
        '#',
        ';',
        '\'',
        '`',
        ',',
        '.',
        '/',
        'CapsLock',
        'F1',
        'F2',
        'F3',
        'F4',
        'F5',
        'F6',
        'F7',
        'F8',
        'F9',
        'F10',
        'F11',
        'F12',
        'PrintScreen',
        'ScrollLock',
        'Pause',
        'Insert',
        'Home',
        'PageUp',
        'Del',
        'End',
        'PageDown',
        'Right',
        'Left',
        'Down',
        'Up',
        'NumLock',
        'KP/',
        'KP*',
        'KP-',
        'KP+',
        'KPEnter',
        'KP1',
        'KP2',
        'KP3',
        'KP4',
        'KP5',
        'KP6',
        'KP7',
        'KP8',
        'KP9',
        'KP0',
        'KP.',
        None,  # non-us \ and |
        'App', # Application
        'Power',
        'KP=',
        'F13',
        'F14',
        'F15',
        'F16',
        'F17',
        'F18',
        'F19',
        'F20',
        'F21',
        'F22',
        'F23',
        'F24',
        'Execute',
        'Help',
        'Menu',
        'Select',
        'Stop',
        'Again',
        'Undo',
        'Cut',
        'Copy',
        'Paste',
        'Find',
        'Mute',
        'VolUp',
        'VolDown',
        None,
        None,
        None, # Locking Caps, Num, Scroll
        'KP,',
        None, # KP=
        None, # KB Int 1
        None,
        None,
        None,
        None,
        None,
        None,
        None,
        None, # KB int 9
        None, # KB Lang 1
        None,
        None,
        None,
        None,
        None,
        None,
        None,
        None, # KB Lang 9
        None, # alt erase
        'SysRq',
        'Cancel',
        'Clear',
        'Prior',
        'Return',
        'Separator',
        'Out',
        'Oper',
        None, # clear / again
        None, # crSel / props
        None, # exsel
        None, # 165 - reserved
        None,
        None,
        None,
        None,
        None,
        None,
        None,
        None,
        None,
        None, # 175 - reserved
        'KP00',
        'KP000',
        None, # thousand sep
        None, # decimal sep
        None, # currency
        None, # currency sub
        'KP(',
        'KP)',
        'KP{',
        'KP}',
        'KPTab',
        'KPBackspace',
        'KPA',
        'KPB',
        'KPC',
        'KPD',
        'KPE',
        'KPF',
        'KPXOR',
        'KP^',
        'KP%',
        'KP<',
        'KP>',
        'KP&',
        'KP&&',
        'KP|',
        'KP||',
        'KP:',
        'KP#',
        'KPSpace',
        'KP@',
        'KP!',
        None, # KP MS
        None, # KP MR
        None, # KP MC
        None, # KP M+
        None, # KP M-
        None, # KP M*
        None, # KP M/
        None, # KP +/-
        None, # KP Clear
        None, # KP Clear entry
        None, # KP Bin
        None, # KP Oct
        None, # KP Dec
        None, # KP Hex
        None, # Reserved
        None, # Reserved
        'LControl',
        'LShift',
        'LAlt',
        'LGUI',
        'RControl',
        'RShift',
        'RAlt',
        'RGUI',
    ]
    MOMENTARY_OFFSET = 42;

    # L<n>[*+]?: Layer <n>. * => one-shot; + => toggle; none => momentary
    # Modifier* => oneshot

    def OSL(self, layer):
        # 0xc000 + 8
        n = 0xc008 + layer
        kc = n % 256
        flags = (n - kc) / 256
        return "{0} {1}".format (flags, kc)

    def Layer(self, n):
        return "68 {0}".format (n)

    def OSM(self, mod):
        n = 0xc000 + self.__keyCodes.index (mod) - self.__keyCodes.index ('LControl')
        kc = n % 256
        flags = (n - kc) / 256
        return "{0} {1}".format (flags, kc)

    def toCode(self, key):
        if key == 'XXX':
            return "0 0"
        if key == '---':
            return '255 255';
        if key in self.__keyCodes:
            return "0 {0}".format (self.__keyCodes.index (key))

        # Layer keys
        if re.match ("L(\d+)([\*\+]?)", key):
            (layer, method) = re.search ("L(\d+)([\*\+]?)", key).groups ()
            if method == "*":   # OneShot
                return self.OSL(int(layer))
            elif method == "+": # toggle
                return self.Layer(int(layer))
            else:               # momentary
                return self.Layer(int(layer) + self.MOMENTARY_OFFSET)

        # OSM
        if re.match("([LR](Control|Shift|Alt|GUI))\*", key):
            mod = re.search("([LR](Control|Shift|Alt|GUI))\*", key).groups()[0]
            return self.OSM(mod)

        return key

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
