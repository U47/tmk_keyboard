/*
Copyright 2016-19 Jun Wako <wakojun@gmail.com>
Copyright 2025 Rony Ballouz

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
#include "unimap_common.h"

#define AC_FN0 ACTION_LAYER_MODS(1, MOD_LSFT)
#define AC_FN1 ACTION_LAYER_TOGGLE(2)


#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
/* Sharp X1 Turbo Keyboard Layout
 * ,------------------------------------------------------------------------------------------.  ,-----------------------.
 * |  F1/6  |  F2/7  |  F3/8  |  F4/9  | F5/10  | |PgUP|PgDN| |F11 |F12 | |                   |  | END |  /  |  *  |  -  |
 * |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|  | HOME|     |     |     |
 * |------------------------------------------------------------------------------------------|  |-----------------------|
 * | ESC |  !  |  "  |  #  |  $  |  %  |  &  |  '  |  (  |  )  |    | =  |  ¯ |  |  |  BSPC   |  |  7  |  8  |  9  |  +  |
 * |     |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 0  | -  |  ^ |  ¥  |         |  |     |     |     |     |
 * |------------------------------------------------------------------------------------------|  |-----------------------|
 * |  H TAB |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  | `  | {  | INS |     |  |  4  |  5  |  6  |  =  |
 * |        |     |     |     |     |     |     |     |     |     |     | @  | [  | DEL |  |  |  |     |     |     |     |
 * |------------------------------------------------------------------------------------'  |  |  |-----------------------|
 * |   Ctrl  |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  +  |  *  |  }  |  ←─-|  |  |  1  |  2  |  3  |  .  |
 * |         |     |     |     |     |     |     |     |     |     |  ;  |  :  |  ]  |        |  |     |     |     |     |
 * |------------------------------------------------------------------------------------------|  |-----------------------|
 * |    SHIFT   |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  <  |  >  |  ?  |  _  |   SHIFT   |  |  0  |  ,  |  ↑  |  |  |
 * |            |     |     |     |     |     |     |     |  ,  |  .  |  /  |     |           |  |     |     |     |  |  |
 * `------------------------------------------------------------------------------------------'  |-----------------|  |  |
 *             |Cmd  | CAPS|                      Space                   |Opt |F24 |            |  ←  |  →  |  ↓  | ←─  |
 *             |     | LOCK|                                              |    |    |            |     |     |     |     |
 *             `--------------------------------------------------------------------'            `-----------------------' 
 */
    [0] = UNIMAP_X1( \
    F1, F2, F3, F4, F5,  PGUP,PGDN, F11,F12,                          HOME,PSLS,PAST,PMNS, \
    ESC, 1,  2,  3,  4, 5, 6, 7, 8, 9,  0,  MINS,   EQL, JPY, BSPC,   P7,  P8,  P9,  PPLS, \
    TAB, Q,  W,  E,  R, T, Y, U, I, O,  P,  LBRC,  RBRC, DEL,  ENT,   P4,  P5,  P6,  PEQL, \
    LCTRL,  A,  S,  D,  F, G, H, J, K, L,  SCLN, QUOT, NUHS,          P1,  P2,  P3,  PDOT, \
    FN0,   Z,  X,  C,  V, B, N, M, COMM, DOT, SLSH, RO,               P0,  PCMM,UP,  PENT, \
    LGUI, LCAP,                  SPC,                    LALT, FN1,   LEFT,RIGHT,DOWN      \
    ),

    [1] = UNIMAP_X1( \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS,                                             END, TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  TRNS,  TRNS,   TRNS, TRNS, TRNS,     TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  TRNS,  TRNS,  TRNS,  INS,  TRNS,     TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  TRNS, TRNS, TRNS,                    TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,                           TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,                  TRNS,                   TRNS, TRNS,                                     TRNS,TRNS,TRNS       \
    ),
    [2] = UNIMAP_X1( \
    F6, F7, F8, F9, F10,  TRNS,TRNS, TRNS,TRNS,                                                          TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  TRNS,  TRNS,   TRNS, TRNS, TRNS,     TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,    TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,  TRNS, TRNS, TRNS,                    TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,                           TRNS,TRNS,TRNS,TRNS, \
    TRNS, TRNS,                  TRNS,                   TRNS, TRNS,                                     TRNS,TRNS,TRNS       \
    ),
    
};
