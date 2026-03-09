/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H


#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x1000
#define DEVICE_VER      0x0001
#define MANUFACTURER    t.m.k.
#define PRODUCT         Sharp X1 keyboard converter
#define DESCRIPTION     connect Sharp X1 keyboard to USB

/* matrix size - 19x16 to accommodate JIS X 0201 + modifiers+game */
#define MATRIX_ROWS 19  // 16 for JIS X 0201 + 3 for modifiers, numkeys, gamekeys
#define MATRIX_COLS 16  // Full 4-bit range
#define MATRIX_ROW(code)    ((code)>>4&0x0F)
#define MATRIX_COL(code)    ((code)&0x0F)

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* X1 port setting */
#define X1_PORT        PORTD
#define X1_PIN         PIND
#define X1_DDR         DDRD
#define X1_DATA_BIT    0

/* key combination for command */
#ifndef __ASSEMBLER__
#include "x1.h"
#include "matrix.h"
// Use CTRL key for command mode
#define IS_COMMAND() ( \
   matrix_is_on(MATRIX_ROW(0x10), MATRIX_COL(0x00)) \
)
#endif

#endif