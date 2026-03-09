/*
Copyright 2011 Jun Wako <wakojun@gmail.com>
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

/*
 * scan matrix for Sharp X1 keyboard
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "print.h"
#include "debug.h"
#include "x1.h"
#include "matrix.h"

// key matrix (1:on, 0:off)
// 19x16 matrix: 16 rows for JIS X 0201 + modifiers + game 
static matrix_row_t matrix[MATRIX_ROWS];

// LUT checks if scan code corresponds to mode B key
// Store bit numbers (1-24, or 0 for "not a game key")
static const uint8_t PROGMEM ascii_to_mode_b_bit[256] = {
    [0x11] = 24, [0x51] = 24, [0x71] = 24, [0xC0] = 24, [0xE0] = 24,                // Q   -> bit 23
    [0x17] = 23, [0x77] = 23, [0x57] = 23, [0xC3] = 23, [0xE1] = 23,                // W   -> bit 22
    [0x05] = 22, [0x45] = 22, [0x65] = 22, [0xA8] = 22, [0xB2] = 22, [0xE2] = 22,   // E   -> bit 21
    [0x01] = 21, [0x41] = 21, [0x61] = 21, [0x7F] = 21, [0xC1] = 21,                // A   -> bit 20
    [0x04] = 20, [0x44] = 20, [0x64] = 20, [0xBC] = 20, [0xEA] = 20,                // D   -> bit 19
    [0x1A] = 19, [0x5A] = 19, [0x7A] = 19, [0x80] = 19, [0xAF] = 19, [0xC2] = 19,   // Z   -> bit 18
    [0x18] = 18, [0x58] = 18, [0x78] = 18, [0x81] = 18, [0xBB] = 18,                // X   -> bit 17
    [0x43] = 17, [0x63] = 17, [0x81] = 17, [0xBF] = 17,                             // C   -> bit 16
    [0x9A] = 16,                                                                    // P7  -> bit 15
    [0x95] = 15,                                                                    // P4  -> bit 14
    [0x99] = 14,                                                                    // P1  -> bit 13
    [0x93] = 13,                                                                    // P8  -> bit 12
    [0x92] = 12,                                                                    // P2  -> bit 11
    [0x97] = 11,                                                                    // P9  -> bit 10
    [0x94] = 10,                                                                    // P6  -> bit  9
    [0x98] = 9,                                                                     // P3  -> bit  8
    [0x1B] = 8 ,                                                                    // ESC -> bit  7
    [0x21] = 7 , [0x31] = 7 , [0xC7] = 7 , [0xF1] = 7 ,                             // 1   -> bit  6
    [0x9C] = 6 ,                                                                    // -   -> bit  5
    [0x9D] = 5 ,                                                                    // +   -> bit  4 
    [0x9B] = 4 ,                                                                    // *   -> bit  3
    [0x09] = 3 ,                                                                    // TAB -> bit  2
    [0x20] = 2 ,                                                                    // SPC -> bit  1
    [0x0D] = 1 ,                                                                    // RET -> bit  0
    // all other entries /should/ be 0x00
};

// Lookup table to remap numkeys to their matrix positions
// three cases: 1. scan code between 2A & 39, 2. ascii==3D, or 3. ascii==0x0D
// case 1 indexes the table directly with (ascii-2A), case 2 uses lut[16], case 3 manual.
static const uint8_t num_key_lut[17] = {
    0x9B, // *
    0x9D, // +
    0x9F, // ,
    0x9C, // -
    0x91, // .
    0x9E, // /
    0x8F, // 0
    0x99, // 1
    0x92, // 2
    0x98, // 3
    0x95, // 4
    0x96, // 5
    0x94, // 6
    0x9A, // 7
    0x93, // 8
    0x97, // 9
    0x90, // PEQ  
};

// Key state tracking for "last key wins" behavior
static uint8_t last_ascii_key = 0;
bool high_key = false;


// Mode B keys tracking for mode A detection.
static uint32_t last_mode_b_data =0;

// Mode tracking for game key overrides and kana shuffling detection
static bool keyboard_mode_b = false;

// This maps Mode B kana codes to their correct physical key positions
static const uint8_t mode_b_kana_lut[4][16] = {
    // Row A: 0xA0-0xAF
    // JPY, SLSH, LBRC, RBRC,  DOT,   RO, SLSH,    1,    2,    3,    4,    5,    N,    M, CMMA,    C, 
    //    ,    ｡,    ｢,    ｣,   ､ ,    ･,    ｦ,    ｧ,    ｨ,    ｩ,    ｪ,    ｫ,    ｬ,    ｭ,    ｮ,    ｯ
    { 0xA0, 0xA5, 0xA2, 0xA3, 0xDF, 0xDB, 0xD2, 0xC7, 0xCC, 0xA7, 0xA9, 0xAA, 0xD0, 0xD3, 0xA4, 0xBF },
    // Row B: 0xB0-0xBF  
    //QUOT,    1,    2,    3,    4,    5,    Q,    W,    E,    R,    T,    A,    S,    D,    F,    G,
    //  ｰ ,    ｱ,    ｲ,    ｳ,    ｴ,    ｵ,    ｶ,    ｷ,    ｸ,    ｹ,    ｺ,    ｻ,    ｼ,    ｽ,    ｾ,    ｿ 
    { 0xB9, 0xC7, 0xCC, 0xB1, 0xB3, 0xB4, 0xC0, 0xC3, 0xB2, 0xBD, 0xB6, 0xC1, 0xC4, 0xBC, 0xCA, 0xB7 },
    // Row C: 0xC0-0xCF
    //   Z,    X,    C,    V,    B,    6,    7,    8,    9,    0,    Y,    U,    I,    O,    P,    H,
    //   ﾀ,    ﾁ,    ﾂ,    ﾃ,    ﾄ,    ﾅ,    ﾆ,    ﾇ,    ﾈ,    ﾉ,    ﾊ,     ﾋ,   ﾌ,    ﾍ,    ﾎ,    ﾏ
    { 0xC2, 0xBB, 0xBF, 0xCB, 0xBA, 0xB5, 0xD4, 0xD5, 0xD6, 0xDC, 0xDD, 0xC5, 0xC6, 0xD7, 0xBE, 0xB8 },
    // Row D: 0xD0-0xDF (example: 0xD2 -> L key, 0xD8 -> normal L kana position)
    //   J,    K,    L,  SCN,    N,    M, CMMA,  MNS,  EQL,  JPY,  GRV, LBRC,  DOT,   RO, QUOT, RBRC,
    //   ﾐ,    ﾑ,    ﾒ,    ﾓ,    ﾔ,    ﾕ,    ﾖ,    ﾗ,    ﾘ,    ﾙ,    ﾚ,    ﾛ,    ﾜ,    ﾝ,    ﾞ,    ﾟ
    { 0xCF, 0xC9, 0xD8, 0xDA, 0xD0, 0xD3, 0xC8, 0xCE, 0xCD, 0xB0, 0xDE, 0xDF, 0xD9, 0xDB, 0xB9, 0xA3 }
};

////////////////////////////////////////////////////////////////////////////////
// Matrix manipulation functions
////////////////////////////////////////////////////////////////////////////////

static void clear_all_keys(void){
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }
}

static void register_status(uint8_t status){
    //updates matrix with lower five bits of status to set modifier keys
    matrix[16] = (matrix[16] & 0xFFE0u) | ((uint16_t)(~status) & 0x001Fu);
}

static void register_key(uint8_t ascii_code){
    // JIS X 0201 positions - ASCII code directly maps to matrix
    uint8_t col = ascii_code & 0x0F;        
    uint8_t row = (ascii_code >> 4) & 0x0F; 
    matrix[row] |= (1 << col);

    last_ascii_key = ascii_code;
}

static void clear_key(uint8_t ascii_code){
    // JIS X 0201 positions - ASCII code directly maps to matrix
    uint8_t col = ascii_code & 0x0F;        
    uint8_t row = (ascii_code >> 4) & 0x0F; 
    matrix[row] &= ~(1 << col);
}

static void clear_high_keys(void){
    //clear non-game, non-status high keys
    matrix[0x10]&=(0x001Fu);            //clear command keys and arrows
    //matrix[0x11] contains only mode B keys
    matrix[0x12]&=(0x00FFu);            //clear function keys
}

static void register_high_key(uint8_t address){
    uint8_t col = address & 0xF;
    uint8_t row = (address >> 4) & 0x0F;
    matrix[row+0x10]|= (1<<col);
}

static void register_game_keys(uint32_t game_keys){
    //put the lower 16 bits of gamekeys into row 17, the upper 8 bits into row 18
    matrix[0x11]= (uint16_t)(game_keys&0xFFFF);
    matrix[0x12]= (matrix[0x12]&0xFF00u) | (uint16_t)((game_keys>>16) &0x00FF);
}

////////////////////////////////////////////////////////////////////////////////
// X1 Protocol helper functions
////////////////////////////////////////////////////////////////////////////////

static void handle_mode_b_game_keys(bool mode_b, uint32_t game_keys)
{
    if (!(mode_b)) return;
    
    // Set/clear game key positions based on Mode B packet
    register_game_keys(game_keys);

    // Update last Mode B data for mode detection
    last_mode_b_data = game_keys;
    if (debug_enable) {
        xprintf("    game_keys: 0x%06lX\n", game_keys);
    }
}

static void mode_a_detect(uint8_t ascii_code, uint8_t status)
{
    // Mode A packet - check if we should switch from Mode B back to Mode A
    if (keyboard_mode_b) {
        uint8_t mode_b_bit = pgm_read_byte(&ascii_to_mode_b_bit[ascii_code]);
        
        if (mode_b_bit > 0) {
            // This ASCII could be a Mode B game key - check if it was in the last Mode B packet
            uint32_t bit_mask = (1UL << (mode_b_bit - 1));  // Convert 1-based to 0-based bit
            
            if (!(last_mode_b_data & bit_mask)) {
                // Game key not in Mode B packet - this could be Mode A
                
                // Need to ensure CTRL is not pressed for ESC, ENTER, and HTAB 
                if ((ascii_code == 0x1B || ascii_code == 0x0D || ascii_code == 0x09) && 
                    (status & X1_STATUS_CTRL)) {
                    keyboard_mode_b = false;
                    if (debug_enable) {
                        xprintf("X1: Switched to Mode A (ENTER/TAB/ESC key detected)\n");
                    }
                } else if (ascii_code != 0x1B && ascii_code != 0x0D && ascii_code != 0x09) {
                    // cannot judge ESC/ENTER/TAB game keys if CTRL pressed, 
                    // because they have non-game key combinations that will produce the scancode.
                    keyboard_mode_b = false;
                    if (debug_enable) {
                        xprintf("X1: Switched to Mode A (game key w/o Mode B packet)\n");
                    }
                }
            }
        }
    }
}

static void handle_ascii_key_press(uint8_t ascii_code, uint8_t status)
{
    //ignore repeat keypress
    bool is_repeat = ((status & X1_STATUS_REPEAT) == 0);  // Active-low
    if(is_repeat) goto set_modifiers;

    //new key always clears the old ones
    if(!high_key){
        clear_key(last_ascii_key);
    }else{
        clear_high_keys();
        high_key=false;
    }

    bool key_registered = false;

    if(ascii_code || ((status & (X1_STATUS_KEYINPUT | X1_STATUS_CTRL))==0)){
        
        // Num Key reassignment to unique scancodes, registers TenKey special cases
        // doesn't register normal numkeys to allow for mode A detection.
        bool is_tenkey = ((status & X1_STATUS_TENKEY) == 0);
        if (is_tenkey) {
            switch (ascii_code) {
                case 0x2A ... 0x39: // 0 to 9 or + - * , .
                    ascii_code = num_key_lut[ascii_code - 0x2A];
                    break;
                case 0x3D: // Equal sign
                    ascii_code = num_key_lut[0x10];
                    break;
            // the following cases register keys since they are neither kana nor mode B keys.
                case 0xE1: // Copy
                    register_high_key(0x05);
                    key_registered = high_key = true;
                    break;
                case 0xE2: // Help
                    register_high_key(0x06);
                    key_registered = high_key = true;
                    break;
                case 0xE8: // XFER
                    register_high_key(0x07);
                    key_registered = high_key = true;
                    break;
                case 0xEB: // Roll Up
                    register_high_key(0x08);
                    key_registered = high_key = true;
                    break;
                case 0xEC: // Roll Down
                    register_high_key(0x09);
                    key_registered = high_key = true;
                    break;
                case 0x0D: // Numpad Enter
                    mode_a_detect(ascii_code, status);
                    if(!keyboard_mode_b) register_high_key(0x0A);
                    key_registered = high_key = true;
                    break;
                case 0x0B ... 0x0C: // HOME/CLR
                    register_high_key(0x0B);
                    key_registered = high_key = true;
                    break;
                case 0x1C ... 0x1F: // Arrows
                    register_high_key(ascii_code&0x0F);
                    key_registered = high_key = true;
                    break;
                case 0x71 ... 0x7A: // Function keys
                    if (ascii_code>0x75) ascii_code-=5;
                    register_high_key(0x28+(ascii_code&0x0F));
                    key_registered = high_key = true;
                    break;
                default:
                    break;
            }
        }else if ((status&(X1_STATUS_CTRL|X1_STATUS_SHIFT))==X1_STATUS_SHIFT){
            // if CTRL held without SHIFT
            switch (ascii_code) {
                case 0x03: // resolve CTRL+C collision with BREAK
                    register_key(0x43);
                    key_registered=true;
                    break;
                case 0x12: // resolve CTRL+R collison with INS
                    register_key(0x52);
                    key_registered=true;
                    break;
                default:
                    break;
            }
        }

        if (!key_registered){

            uint8_t pre_shuffle = ascii_code;
            // Kana scancodes in Mode B need to be remapped
            if (keyboard_mode_b && 
                ((status & X1_STATUS_KANA) == 0) &&  // KANA mode active (active-low)
                (ascii_code >= 0xA0 && ascii_code <= 0xDF)) {
                
                // Use lookup table for Mode B kana shuffling
                uint8_t lut_row = ((ascii_code >> 4) & 0x0F) - 0x0A;  // 0xA0->0, 0xB0->1, etc.
                uint8_t lut_col = ascii_code & 0x0F;
                ascii_code = mode_b_kana_lut[lut_row][lut_col]; // tentative ascii code reassignment
            }

            // Mode detection and switching
            mode_a_detect(ascii_code, status);
            
            //if switched to mode A ignore the kana shuffle -- decisive ascii code update
            if (!keyboard_mode_b){
                ascii_code = pre_shuffle;
            } else {
                // If mode A scancode corresponds to Mode B game key, let Mode B data override
                // also ignore the false positives : CTRL+M(=ENTER), CTRL+[(=ESC), and CTRL+I(=TAB)
                if ( pgm_read_byte(&ascii_to_mode_b_bit[ascii_code])> 0) {
                    if (debug_enable) {
                        xprintf("X1: Ignoring Mode A ASCII 0x%02X - Mode B override active\n", ascii_code);
                    }
                    goto set_modifiers;
                }
            }

            //can now safely set key 
            register_key(ascii_code);
        }
    }
    set_modifiers:
    register_status(status);
    
    if (debug_enable) {
        xprintf("    Pressed ASCII:0x%02X%s%s\n", 
                ascii_code,
                is_repeat ? " [repeat]" : "",
                (status & X1_STATUS_TENKEY) ? "" : " [tenkey]");
    }
}

////////////////////////////////////////////////////////////////////////////////
// Main processing function
////////////////////////////////////////////////////////////////////////////////

void hook_late_init(void){
    debug_enable = true;
    x1_host_init();
    
    // LED on
    DDRD |= (1<<6); PORTD |= (1<<6);

    // Clear matrix
    clear_all_keys();
    
    last_mode_b_data=0;
    last_ascii_key=0x00;
    high_key=false;
    
    if (debug_enable) {
        xprintf("X1 keyboard initialized\n");
    }

    // LED off
    DDRD |= (1<<6); PORTD &= ~(1<<6);
    return;
}

static void process_x1_key(x1_data_t* data)
{
    if (data->mode_b) {
        // Process Mode B packet - game keys only
        keyboard_mode_b = true;
        if (debug_enable) {
            xprintf("X1: Mode B packet received\n");
        }
        handle_mode_b_game_keys(data->mode_b, data->game_keys);
    } else {
        // Process Mode A packet - ASCII keys and modifiers
        if (debug_enable) {
            xprintf("X1: Status:%02X ASCII:%02X\n", 
                    data->status, data->ascii);
        }
        handle_ascii_key_press(data->ascii, data->status);
    }
}

void matrix_init(void)
{
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

uint8_t matrix_scan(void)
{
    x1_data_t x1_data;
    uint8_t changed = 0;
    
    // Check for received X1 data
    if (x1_host_recv(&x1_data)) {
        process_x1_key(&x1_data);
        changed = 1;
    }
    
    return changed;
}