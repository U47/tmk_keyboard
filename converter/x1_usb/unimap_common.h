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
#ifndef UNIMAP_COMMON_H
#define UNIMAP_COMMON_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "unimap.h"


/* Sharp X1 Turbo Keyboard Layout
 * ,------------------------------------------------------------------------------------------.  ,-----------------------.
 * |   F1   |   F2   |   F3   |   F4   |   F5   | |R UP|R DN| |HELP|COPY| |                   |  | CLR |  /  |  *  |  -  |
 * |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|  | HOME|     |     |     |
 * |------------------------------------------------------------------------------------------|  |-----------------------|
 * | ESC |  !  | "   |  #  |  $  |  %  |  &  |  '  |  (  |  )  |    | =  |  ¯ |  |  |  Break  |  |  7  |  8  |  9  |  +  |
 * |     |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  | 0  | -  |  ^ |  ¥  |         |  |     |     |     |     |
 * |------------------------------------------------------------------------------------------|  |-----------------------|
 * |  H TAB |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  | `  | {  | INS |     |  |  4  |  5  |  6  |  =  |
 * |        |     |     |     |     |     |     |     |     |     |     | @  | [  | DEL |  |  |  |     |     |     |     |
 * |------------------------------------------------------------------------------------'  |  |  |-----------------------|
 * |   CTRL  |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  +  |  *  |  }  |  ←─-|  |  |  1  |  2  |  3  |  .  |
 * |         |     |     |     |     |     |     |     |     |     |  ;  |  :  |  ]  |        |  |     |     |     |     |
 * |------------------------------------------------------------------------------------------|  |-----------------------|
 * |    SHIFT   |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  <  |  >  |  ?  |  _  |   SHIFT   |  |  0  |  ,  |  ↑  |  |  |
 * |            |     |     |     |     |     |     |     |  ,  |  .  |  /  |     |           |  |     |     |     |  |  |
 * `------------------------------------------------------------------------------------------'  |-----------------|  |  |
 *             |GRAPH| CAPS|                      Space                   |Kana|XFER|            |  ←  |  →  |  ↓  | ←─  |
 *             |     | LOCK|                                              |    |    |            |     |     |     |     |
 *             `--------------------------------------------------------------------'            `-----------------------' 
 *
 *   Universal 128-key keyboard layout(8x16)
 *         ,-----------------------------------------------.
 *         |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
 * ,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
 * |Esc|   |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|     |PrS|ScL|Pau|     |VDn|VUp|Mut|
 * `---'   `-----------------------------------------------'     `-----------'     `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  -|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  +|
 * |-----------------------------------------------------------| `-----------' |---------------|
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Entr|               |  4|  5|  6|KP,|
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|
 * |-----------------------------------------------------------| ,-----------. |---------------|
 * |Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  0    |  .|KP=|
 * `-----------------------------------------------------------' `-----------' `---------------'
 * 
 */
#define UNIMAP_X1( \
    KF1,KF2,KF3,KF4,KF5,  KRU,KRD, KHP,KCP,                       KHC,KDV,KML,KMN, \
    KES,K1, K2, K3, K4, K5, K6, K7, K8, K9, K0, KMI,KCT,KYN,KBR,  K7N,K8N,K9N,KPL, \
    KHT,KQ, KW, KE, KR, KT, KY, KU, KI, KO, KP, KAT,KLB,KID,KCR,  K4N,K5N,K6N,KEQ, \
    CTL,KA, KS, KD, KF, KG, KH, KJ, KK, KL, KSC,KCN,KRB,          K1N,K2N,K3N,KPT, \
    KSH,KZ, KX, KC, KV, KB, KN, KM, KCM,KPR,KSL,KRO,              K0N,KAP,KUP,KNR, \
    KGR,KCA,                KSP,                 KKA,KXF,         KLF,KRI,KDN      \
) UNIMAP( \
            NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, KXF,                                     \
    KES,    KF1,KF2,KF3,KF4,KF5,NO, NO, NO, NO, NO, KHP,KCP,      NO, NO, NO,       NO, NO, NO,  \
    KAT,K1, K2, K3, K4, K5, K6, K7, K8, K9, K0, KMI,KCT,KYN,KBR,  NO, KHC,KRU,  NO, KDV,KML,KMN, \
    KHT,KQ, KW, KE, KR, KT, KY, KU, KI, KO, KP, KLB,KRB,    NO,   KID,NO, KRD,  K7N,K8N,K9N,KPL, \
    KCA,KA, KS, KD, KF, KG, KH, KJ, KK, KL, KSC,KCN,    NO, KCR,                K4N,K5N,K6N,KAP, \
    KSH,NO, KZ, KX, KC, KV, KB, KN, KM, KCM,KPR,KSL,    KRO,KSH,      KUP,      K1N,K2N,K3N,KNR, \
    KGR,NO, CTL,NO,         KSP,        NO, KKA,CTL,NO, NO, KGR,  KLF,KDN,KRI,  K0N,    KPT,KEQ  \
)


// JIS X 0201 based matrix - ASCII codes map directly to matrix positions
// 18x16 matrix: rows 0-15 for JIS X 0201, row 16 for ctrl keys, row 17 for numpad
// SHIFT and GRAPH combos are handled by the matrix. 
// CTRL and NUM collision detection is external, so is Mode B kana shuffling.
const uint8_t PROGMEM unimap_trans[MATRIX_ROWS][MATRIX_COLS] = {
    // Row 0: 0x00-0x0F (Control characters)
    //need to watch out for ctrl+tab and ctr+enter
    {
        UNIMAP_GRV,             // 0x00 CTRL+@ if key_in set, or release
        UNIMAP_A,               // 0x01 CTRL+A 
        UNIMAP_B,               // 0x02 CTRL+B
        UNIMAP_BSPC,            // 0x03 CTRL+C or SHIFT+BREAK or ctrl+break collision (or ctrl+shft+brk)
        UNIMAP_D,               // 0x04 CTRL+D
        UNIMAP_E,               // 0x05 CTRL+E
        UNIMAP_F,               // 0x06 CTRL+F
        UNIMAP_G,               // 0x07 CTRL+G
        UNIMAP_DEL,             // 0x08 DEL (without SHIFT) or CTRL+H
        UNIMAP_TAB,             // 0x09 HTAB or CTRL+I
        UNIMAP_J,               // 0x0A CTRL+J
        UNIMAP_K,               // 0x0B CTRL+K / HOME (without SHIFT)  
        UNIMAP_L,               // 0x0C CTRL+L / CLR (with SHIFT, same physical key)
        UNIMAP_ENTER,           // 0x0D CR or CTRL+M
        UNIMAP_N,               // 0x0E CTRL+N
        UNIMAP_O,               // 0x0F CTRL+O
    },
    // Row 1: 0x10-0x1F (More control characters)
    {
        UNIMAP_P,               // 0x10 CTRL+P
        UNIMAP_Q,               // 0x11 CTRL+Q
        UNIMAP_DEL,             // 0x12 CTRL+R or INS (with SHIFT+DEL)
        UNIMAP_BSPC,            // 0x13 BREAK or CTRL+S
        UNIMAP_T,               // 0x14 CTRL+T
        UNIMAP_U,               // 0x15 CTRL+U
        UNIMAP_V,               // 0x16 CTRL+V
        UNIMAP_W,               // 0x17 CTRL+W
        UNIMAP_X,               // 0x18 CTRL+X
        UNIMAP_Y,               // 0x19 CTRL+Y
        UNIMAP_Z,               // 0x1A CTRL+Z
        UNIMAP_ESC,             // 0x1B ESC or ^[
        UNIMAP_JPY,             // 0x1C ^¥ right
        UNIMAP_RBRC,            // 0x1D ^] left
        UNIMAP_EQL,             // 0x1E ^^ up 
        UNIMAP_RO,              // 0x1F ^_ down
    },
    // Row 2: 0x20-0x2F (Space and symbols)
    {
        UNIMAP_SPC,             // 0x20 Space
        UNIMAP_1,               // 0x21 ! (SHIFT+1)
        UNIMAP_2,               // 0x22 " (SHIFT+2)
        UNIMAP_3,               // 0x23 # (SHIFT+3)
        UNIMAP_4,               // 0x24 $ (SHIFT+4)
        UNIMAP_5,               // 0x25 % (SHIFT+5)
        UNIMAP_6,               // 0x26 & (SHIFT+6)
        UNIMAP_7,               // 0x27 ' (SHIFT+7)
        UNIMAP_8,               // 0x28 ( (SHIFT+8)
        UNIMAP_9,               // 0x29 ) (SHIFT+9)
        UNIMAP_QUOTE,           // 0x2A * (SHIFT+:)
        UNIMAP_SCLN,            // 0x2B + (SHIFT+;)
        UNIMAP_COMM,            // 0x2C ,
        UNIMAP_MINS,            // 0x2D -
        UNIMAP_DOT,             // 0x2E .
        UNIMAP_SLSH,            // 0x2F /
    },
    // Row 3: 0x30-0x3F (Numbers and symbols)
    {
        UNIMAP_0,               // 0x30 0
        UNIMAP_1,               // 0x31 1
        UNIMAP_2,               // 0x32 2
        UNIMAP_3,               // 0x33 3
        UNIMAP_4,               // 0x34 4
        UNIMAP_5,               // 0x35 5
        UNIMAP_6,               // 0x36 6
        UNIMAP_7,               // 0x37 7
        UNIMAP_8,               // 0x38 8
        UNIMAP_9,               // 0x39 9
        UNIMAP_QUOTE,           // 0x3A :
        UNIMAP_SCLN,            // 0x3B ;
        UNIMAP_COMM,            // 0x3C < (SHIFT+,)
        UNIMAP_MINS,            // 0x3D = (SHIFT+-)
        UNIMAP_DOT,             // 0x3E > (SHIFT+.)
        UNIMAP_SLSH,            // 0x3F ? (SHIFT+/)
    },
    // Row 4: 0x40-0x4F (@ and uppercase A-O)
    {
        UNIMAP_GRV,             // 0x40 @ (at symbol key on X1)
        UNIMAP_A,               // 0x41 A
        UNIMAP_B,               // 0x42 B  
        UNIMAP_C,               // 0x43 C
        UNIMAP_D,               // 0x44 D
        UNIMAP_E,               // 0x45 E
        UNIMAP_F,               // 0x46 F
        UNIMAP_G,               // 0x47 G
        UNIMAP_H,               // 0x48 H
        UNIMAP_I,               // 0x49 I
        UNIMAP_J,               // 0x4A J
        UNIMAP_K,               // 0x4B K
        UNIMAP_L,               // 0x4C L
        UNIMAP_M,               // 0x4D M
        UNIMAP_N,               // 0x4E N
        UNIMAP_O,               // 0x4F O
    },
    // Row 5: 0x50-0x5F (Uppercase P-Z and symbols)
    {
        UNIMAP_P,               // 0x50 P
        UNIMAP_Q,               // 0x51 Q
        UNIMAP_R,               // 0x52 R
        UNIMAP_S,               // 0x53 S
        UNIMAP_T,               // 0x54 T
        UNIMAP_U,               // 0x55 U
        UNIMAP_V,               // 0x56 V
        UNIMAP_W,               // 0x57 W
        UNIMAP_X,               // 0x58 X
        UNIMAP_Y,               // 0x59 Y
        UNIMAP_Z,               // 0x5A Z
        UNIMAP_LBRC,            // 0x5B [
        UNIMAP_JPY,             // 0x5C ¥ 
        UNIMAP_RBRC,            // 0x5D ]
        UNIMAP_EQL,             // 0x5E ^ (caret, on = key)
        UNIMAP_RO,              // 0x5F _ (underscore, SHIFT+-)
    },
    // Row 6: 0x60-0x6F (lowercase a-o)
    {
        UNIMAP_GRV,             // 0x60 ` (grave accent, same as @)
        UNIMAP_A,               // 0x61 a (same key as A)
        UNIMAP_B,               // 0x62 b
        UNIMAP_C,               // 0x63 c
        UNIMAP_D,               // 0x64 d
        UNIMAP_E,               // 0x65 e
        UNIMAP_F,               // 0x66 f
        UNIMAP_G,               // 0x67 g
        UNIMAP_H,               // 0x68 h
        UNIMAP_I,               // 0x69 i
        UNIMAP_J,               // 0x6A j
        UNIMAP_K,               // 0x6B k
        UNIMAP_L,               // 0x6C l
        UNIMAP_M,               // 0x6D m
        UNIMAP_N,               // 0x6E n
        UNIMAP_O,               // 0x6F o
    },
    // Row 7: 0x70-0x7F (lowercase p-z and symbols)
    {
        UNIMAP_P,               // 0x70 p
        UNIMAP_Q,               // 0x71 q
        UNIMAP_R,               // 0x72 r
        UNIMAP_S,               // 0x73 s
        UNIMAP_T,               // 0x74 t
        UNIMAP_U,               // 0x75 u
        UNIMAP_V,               // 0x76 v
        UNIMAP_W,               // 0x77 w
        UNIMAP_X,               // 0x78 x
        UNIMAP_Y,               // 0x79 y
        UNIMAP_Z,               // 0x7A z
        UNIMAP_LBRC,            // 0x7B { (SHIFT+[)
        UNIMAP_JPY,             // 0x7C | (SHIFT+¥)
        UNIMAP_RBRC,            // 0x7D } (SHIFT+])
        UNIMAP_EQL,             // 0x7E ‾ (SHIFT+^)
        UNIMAP_A,               // 0x7F (GRAPH+A)
    },
    // Row 8: 0x80-0x8F GRAPH combinations 
    //(numpad zero lives here)
    {
        UNIMAP_Z,               // 0x80
        UNIMAP_X,               // 0x81 
        UNIMAP_C,               // 0x82 
        UNIMAP_V,               // 0x83 
        UNIMAP_B,               // 0x84 
        UNIMAP_N,               // 0x85 
        UNIMAP_M,               // 0x86 
        UNIMAP_COMM,            // 0x87    
        UNIMAP_DOT,             // 0x88   
        UNIMAP_SCLN,            // 0x89    
        UNIMAP_GRV,             // 0x8A  
        UNIMAP_EQL,             // 0x8B  
        UNIMAP_MINS,            // 0x8C   
        UNIMAP_P,               // 0x8D
        UNIMAP_L,               // 0x8E
        UNIMAP_P0,              // 0x8F 
    },
    // Row 9: 0x90-0x9F GRAPH combinations (numpad area)
    {
        UNIMAP_PEQL,            // 0x90   
        UNIMAP_PDOT,            // 0x91  
        UNIMAP_P2,              // 0x92
        UNIMAP_P8,              // 0x93
        UNIMAP_P6,              // 0x94
        UNIMAP_P4,              // 0x95
        UNIMAP_P5,              // 0x96
        UNIMAP_P9,              // 0x97
        UNIMAP_P3,              // 0x98
        UNIMAP_P1,              // 0x99
        UNIMAP_P7,              // 0x9A
        UNIMAP_PAST,            // 0x9B 
        UNIMAP_PMNS,            // 0x9C 
        UNIMAP_PPLS,            // 0x9D  
        UNIMAP_PSLS,            // 0x9E  
        UNIMAP_PCMM,            // 0x9F  
    },
    // Row A: 0xA0-0xAF (Katakana characters - may be shuffled in Mode B)
    {
        UNIMAP_JPY,             // 0xA0 (unused, but Mode B kana SHIFT+¥ sends this)
        UNIMAP_DOT,             // 0xA1 ｡ (halfwidth katakana)
        UNIMAP_LBRC,            // 0xA2 ｢
        UNIMAP_RBRC,            // 0xA3 ｣
        UNIMAP_COMM,            // 0xA4 ､
        UNIMAP_SLSH,            // 0xA5 ･
        UNIMAP_0,               // 0xA6 ｦ
        UNIMAP_3,               // 0xA7 ｧ
        UNIMAP_E,               // 0xA8 ｨ
        UNIMAP_4,               // 0xA9 ｩ
        UNIMAP_5,               // 0xAA ｪ
        UNIMAP_6,               // 0xAB ｫ
        UNIMAP_7,               // 0xAC ｬ
        UNIMAP_8,               // 0xAD ｭ
        UNIMAP_9,               // 0xAE ｮ
        UNIMAP_Z,               // 0xAF ｯ
    },
   // Row B: 0xB0-0xBF (More Katakana)
    {
        UNIMAP_JPY,             // 0xB0 ｰ 
        UNIMAP_3,               // 0xB1 ｱ 
        UNIMAP_E,               // 0xB2 ｲ 
        UNIMAP_4,               // 0xB3 ｳ 
        UNIMAP_5,               // 0xB4 ｴ 
        UNIMAP_6,               // 0xB5 ｵ 
        UNIMAP_T,               // 0xB6 ｶ 
        UNIMAP_G,               // 0xB7 ｷ 
        UNIMAP_H,               // 0xB8 ｸ 
        UNIMAP_QUOTE,           // 0xB9 ｹ 
        UNIMAP_B,               // 0xBA ｺ 
        UNIMAP_X,               // 0xBB ｻ 
        UNIMAP_D,               // 0xBC ｼ 
        UNIMAP_R,               // 0xBD ｽ 
        UNIMAP_P,               // 0xBE ｾ 
        UNIMAP_C,               // 0xBF ｿ 
    },
    // Row C: 0xC0-0xCF (More Katakana)
    {
        UNIMAP_Q,               // 0xC0 ﾀ 
        UNIMAP_A,               // 0xC1 ﾁ 
        UNIMAP_Z,               // 0xC2 ﾂ 
        UNIMAP_W,               // 0xC3 ﾃ 
        UNIMAP_S,               // 0xC4 ﾄ 
        UNIMAP_U,               // 0xC5 ﾅ 
        UNIMAP_I,               // 0xC6 ﾆ 
        UNIMAP_1,               // 0xC7 ﾇ 
        UNIMAP_COMM,            // 0xC8 ﾈ 
        UNIMAP_K,               // 0xC9 ﾉ 
        UNIMAP_F,               // 0xCA ﾊ 
        UNIMAP_V,               // 0xCB ﾋ 
        UNIMAP_2,               // 0xCC ﾌ 
        UNIMAP_EQL,             // 0xCD ﾍ 
        UNIMAP_MINS,            // 0xCE ﾎ 
        UNIMAP_J,               // 0xCF ﾏ 
    },
    // Row D: 0xD0-0xDF (More Katakana)
    {
        UNIMAP_N,               // 0xD0 ﾐ 
        UNIMAP_RBRC,            // 0xD1 ﾑ 
        UNIMAP_SLSH,            // 0xD2 ﾒ 
        UNIMAP_M,               // 0xD3 ﾓ 
        UNIMAP_7,               // 0xD4 ﾔ 
        UNIMAP_8,               // 0xD5 ﾕ 
        UNIMAP_9,               // 0xD6 ﾖ 
        UNIMAP_O,               // 0xD7 ﾗ 
        UNIMAP_L,               // 0xD8 ﾘ 
        UNIMAP_DOT,             // 0xD9 ﾙ 
        UNIMAP_SCLN,            // 0xDA ﾚ 
        UNIMAP_RO,              // 0xDB ﾛ 
        UNIMAP_0,               // 0xDC ﾜ 
        UNIMAP_Y,               // 0xDD ﾝ 
        UNIMAP_GRV,             // 0xDE ﾞ 
        UNIMAP_LBRC,            // 0xDF ﾟ 
    },
    // Row E: 0xE0-0xEF GRAPH combinations
    {
        UNIMAP_Q,               // 0xE0
        UNIMAP_W,               // 0xE1 
        UNIMAP_E,               // 0xE2 
        UNIMAP_R,               // 0xE3
        UNIMAP_T,               // 0xE4
        UNIMAP_Y,               // 0xE5
        UNIMAP_U,               // 0xE6
        UNIMAP_I,               // 0xE7
        UNIMAP_RBRC,            // 0xE8 
        UNIMAP_S,               // 0xE9
        UNIMAP_D,               // 0xEA
        UNIMAP_F,               // 0xEB 
        UNIMAP_G,               // 0xEC 
        UNIMAP_H,               // 0xED
        UNIMAP_J,               // 0xEE
        UNIMAP_K,               // 0xEF
    },
    // Row F: 0xF0-0xFF GRAPH combinations
    {
        UNIMAP_O,               // 0xF0
        UNIMAP_1,               // 0xF1 
        UNIMAP_2,               // 0xF2 
        UNIMAP_3,               // 0xF3
        UNIMAP_4,               // 0xF4
        UNIMAP_5,               // 0xF5
        UNIMAP_6,               // 0xF6
        UNIMAP_7,               // 0xF7
        UNIMAP_8,               // 0xF8 
        UNIMAP_9,               // 0xF9
        UNIMAP_0,               // 0xFA
        UNIMAP_JPY,             // 0xFB 
        UNIMAP_LBRC,            // 0xFC 
        UNIMAP_QUOTE,           // 0xFD
        UNIMAP_SCLN,            // 0xFE
        UNIMAP_RO,              // 0xFF
    },
    // Row 0x10: Modifier keys (CTRL, SHIFT, KANA, CAPS, GRAPH - matches big-endian status bit order)
    // or if ten-key bit set and scan code 3D (=) or 13 (CR)
    {
        UNIMAP_LALT,            // Col 0: CTRL (status bit 0)
        UNIMAP_LSFT,            // Col 1: SHIFT (status bit 1)  
        UNIMAP_KANA,            // Col 2: KANA (status bit 2)
        UNIMAP_CAPS,            // Col 3: CAPS (status bit 3)
        UNIMAP_LCTL,            // Col 4: GRAPH (status bit 4, mapped to ALT)
        UNIMAP_F12,             // 0x05 -- 0xE1 COPY
        UNIMAP_F11,             // 0x06 -- 0xE2 HELP
        UNIMAP_F24,             // 0x07 -- 0xE8 XFER
        UNIMAP_PGDN,            // 0x08 -- 0xEB ROLL DOWN
        UNIMAP_PGUP,            // 0x09 -- 0xEC ROLL UP
        UNIMAP_KP_ENTER,        // 0x0A -- 0x0D numpad enter
        UNIMAP_HOME,            // 0x0B -- 0x0B/0x0C HOME/CLR
        UNIMAP_RIGHT,           // 0x0C -- 0x1C 
        UNIMAP_LEFT,            // 0x0D -- 0x1D
        UNIMAP_UP,              // 0x0E -- 0x1E
        UNIMAP_DOWN,            // 0x0F -- 0x1F  
    },
    //Row 0x11: GAME keys
    {
        UNIMAP_ENTER,            
        UNIMAP_SPC,             
        UNIMAP_TAB,            
        UNIMAP_PAST,           
        UNIMAP_PPLS,           
        UNIMAP_PMNS,           
        UNIMAP_1,               
        UNIMAP_ESC,            
        UNIMAP_P3,             // 0x
        UNIMAP_P6,             // 0x
        UNIMAP_P9,             // 0x
        UNIMAP_P2,             // 0x 
        UNIMAP_P8,             // 0x 
        UNIMAP_P1,             // 0x
        UNIMAP_P4,             // 0x
        UNIMAP_P7,             // 0x 
    },
    //Row 0x12: GAME keys pt 2
    {
        UNIMAP_C,              // 0x
        UNIMAP_X,              // 0x
        UNIMAP_Z,              // 0x
        UNIMAP_D,              // 0x
        UNIMAP_A,              // 0x
        UNIMAP_E,              // 0x 
        UNIMAP_W,              // 0x 
        UNIMAP_Q,              // 0x
        UNIMAP_NO,       
        UNIMAP_F1,             // Function keys     
        UNIMAP_F2,             //
        UNIMAP_F3,             //
        UNIMAP_F4,             //
        UNIMAP_F5,             //
        UNIMAP_NO,             //
        UNIMAP_NO,             //
    }
};
     
#endif