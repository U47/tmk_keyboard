/*
 * SpaceFN
 * http://geekhack.org/index.php?topic=51069.0
 */
#include "keymap_common.h"


const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0: Default Layer
     * ,-----------------------------------------------------------.
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  `|BSp|
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \|
     * |-----------------------------------------------------------|
     * |Fn30  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Enter   |
     * |-----------------------------------------------------------|
     * |LSft    |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|RSft  |Fn0|
     * `-----------------------------------------------------------'
     *       |Alt|Gui  |         SpaceFN       |Gui  |Alt|
     *       `-------------------------------------------'
     */
    [0] = 
    KEYMAP(ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS, GRV,  \
           FN2, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSPC,       \
           FN30,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,ENT,             \
           LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,RSFT,FN0,             \
                LALT,LGUI,          FN1,                RGUI,RALT),

    /* Layer 1: HHKB mode (HHKB Fn)
     * ,-----------------------------------------------------------.
     * |Pwr| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Ins|Del|
     * |-----------------------------------------------------------|
     * |Caps |   |   |   |   |   |   |   |Psc|Slk|Pus|Up |   |Backs|
     * |-----------------------------------------------------------|
     * |      |VoD|VoU|Mut|   |   |  *|  /|Hom|PgU|Lef|Rig|Enter   |
     * |-----------------------------------------------------------|
     * |        |   |   |   |   |   |  +|  -|End|PgD|Dow|      |   |
     * `-----------------------------------------------------------'
     *       |   |     |                       |     |   |
     *       `-------------------------------------------'
     */
    [1] = 
    KEYMAP(PWR, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,   \
           CAPS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS, UP, TRNS, BSPC,      \
           TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,PAST,PSLS,HOME,PGUP,LEFT,RGHT,PENT,            \
           TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PPLS,PMNS,END, PGDN,DOWN,TRNS,TRNS,            \
                TRNS,TRNS,          SPC,                TRNS,TRNS),

    /* Layer 2: SpaceFN
     * ,-----------------------------------------------------------.
     * |Pwr| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|   |Del|
     * |-----------------------------------------------------------|
     * |     |   |   |Esc|   |   |   |Hom|Up |End|Psc|Slk|Pau|Ins  |
     * |-----------------------------------------------------------|
     * |Fn31  |   |   |   |   |   |PgU|Lef|Dow|Rig|   |   |        |
     * |-----------------------------------------------------------|
     * |        |   |   |   |   |Spc|PgD|`  |~  |   |   |      |   |
     * `-----------------------------------------------------------'
     *       |   |     |                       |     |   |
     *       `-------------------------------------------'
     */
    [2] = 
    KEYMAP(PWR, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS,DEL,   \
           TRNS,TRNS,TRNS,ESC, TRNS,TRNS,TRNS,HOME,UP,  END, PSCR,SLCK,PAUS,INS,        \
           FN31,TRNS,TRNS,TRNS,TRNS,TRNS,PGUP,LEFT,DOWN,RGHT,TRNS,TRNS,TRNS,            \
           TRNS,TRNS,TRNS,TRNS,TRNS,SPC, PGDN,GRV, FN2, TRNS,TRNS,TRNS,NO,              \
                TRNS,TRNS,          TRNS,               TRNS,TRNS),

    /* Layer 3: Virtual numpad
     * ,-----------------------------------------------------------.
     * |   |   |   |   |   |   |   |  7|  8|  9|   |  -|  =|   |   |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |  4|  5|  6|   |Ent|   |     |
     * |-----------------------------------------------------------|
     * |      |   |   |   |   |   |   |  1|  2|  3|  +|  *|        |
     * |-----------------------------------------------------------|
     * |        |   |   |   |   |   |   |  0|   |  .|  /|      |   |
     * `-----------------------------------------------------------'
     *       |   |     |                       |     |   |
     *       `-------------------------------------------'
     */
    [3] = 
    KEYMAP(TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,P7,  P8,  P9,  TRNS,PMNS,PEQL,TRNS,TRNS,  \
           TRNS,TRNS,TRNS,ESC, TRNS,TRNS,TRNS,P4,  P5,  P6,  TRNS,PENT,TRNS,TRNS,       \
           TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PGUP,P1,  P2,  P3,  PPLS,PAST,TRNS,            \
           TRNS,TRNS,TRNS,TRNS,TRNS,SPC, PGDN,P0,  TRNS,PDOT,TRNS,TRNS,NO,              \
                TRNS,TRNS,          TRNS,               TRNS,TRNS),
};


/*
 * user defined action function
 */
/*
 * enum function_id {
    CTRL_SPACE_I,       // Ctrl + Up(SpaceFN) -> PgUp
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
#   define MODS_CTRL_MASK   (MOD_BIT(KC_LCTRL)|MOD_BIT(KC_RCTRL))
    static uint8_t ctrl_space_i_prev_ctrl;

    switch (id) {
        // Ctrl + Up(SpaceFN) -> PgUp
        case CTRL_SPACE_I:
            ctrl_space_i_prev_ctrl = get_mods()&MODS_CTRL_MASK;
            if (record->event.pressed) {
                if (ctrl_space_i_prev_ctrl) {
                    del_mods(ctrl_space_i_prev_ctrl);   // remove Ctrl
                    add_key(KC_PGUP);
                    send_keyboard_report(); // send PgUp without Ctrl
                    add_mods(ctrl_space_i_prev_ctrl);   // return Ctrl but not sent
                } else {
                    add_key(KC_UP);
                    send_keyboard_report();
                }
            } else {
                if (ctrl_space_i_prev_ctrl) {
                    del_key(KC_PGUP);
                    send_keyboard_report();
                } else {
                    del_key(KC_UP);
                    send_keyboard_report();
                }
            }
            break;
    }
}
*/

/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),              // HHKB Fn
    [1] = ACTION_LAYER_TAP_KEY(2, KC_SPACE),      // SpaceFN
//    [2] = ACTION_MODS_KEY(MOD_LSFT, KC_GRV),      // tilde
    [2] = ACTION_LAYER_TAP_KEY(3, KC_TAB),        // Virtual numpad
    [30]= ACTION_MODS_TAP_KEY(MOD_LCTL, KC_BSPC), // LCTRL/BKSP dual role key
    [31]= ACTION_MODS_TAP_KEY(MOD_LCTL, KC_DEL),  // LCTRL/Del dual role key

};
