#include QMK_KEYBOARD_H

enum layers {
    BASE,
    SYM,
    VIM,
    CMUX,
};

#define ESC_VIM LT(VIM, KC_ESC)
#define WORD_L LALT(KC_LEFT)
#define WORD_R LALT(KC_RGHT)
#define LINE_L LGUI(KC_LEFT)
#define LINE_R LGUI(KC_RGHT)
#define DOC_TOP LGUI(KC_UP)
#define DOC_END LGUI(KC_DOWN)
#define UNDO LGUI(KC_Z)
#define REDO LSG(KC_Z)
#define COPY LGUI(KC_C)
#define PASTE LGUI(KC_V)
#define CUT LGUI(KC_X)
#define FIND LGUI(KC_F)
#define FIND_N LGUI(KC_G)
#define FIND_P LAG(KC_G)

// cmux: pane focus follows ~/.config/cmux/cmux.json (rebound to Shift+arrows),
// surfaces follow the same file (Cmd+Opt+[ / ]); everything else is the cmux default.
#define CX_PANE_L LSFT(KC_LEFT)
#define CX_PANE_D LSFT(KC_DOWN)
#define CX_PANE_U LSFT(KC_UP)
#define CX_PANE_R LSFT(KC_RGHT)
#define CX_SURF_P LAG(KC_LBRC)
#define CX_SURF_N LAG(KC_RBRC)
#define CX_WS_P LCG(KC_LBRC)
#define CX_WS_N LCG(KC_RBRC)
#define CX_WS_NEW LGUI(KC_N)
#define CX_WS_GO LGUI(KC_P)
#define CX_WS_DONE LGUI(KC_SCLN)
#define CX_WS_CLOSE LSG(KC_W)
#define CX_TAB_NEW LGUI(KC_T)
#define CX_TAB_CLOSE LGUI(KC_W)
#define CX_TAB_REOPEN LSG(KC_T)
#define CX_TAB_RENAME LGUI(KC_R)
#define CX_SPLIT_R LGUI(KC_D)
#define CX_SPLIT_D LSG(KC_D)
#define CX_ZOOM LSG(KC_ENT)
#define CX_EQUALIZE LCG(LSFT(KC_EQL))
#define CX_RESIZE_L LCTL(LSFT(KC_H))
#define CX_RESIZE_D LCTL(LSFT(KC_J))
#define CX_RESIZE_U LCTL(LSFT(KC_K))
#define CX_RESIZE_R LCTL(LSFT(KC_L))
#define CX_SIDEBAR LGUI(KC_B)
#define CX_RSIDEBAR LAG(KC_B)
#define CX_PALETTE LSG(KC_P)
#define CX_SEARCH LAG(KC_F)
#define CX_NOTIFS LGUI(KC_I)
#define CX_UNREAD LSG(KC_U)
#define CX_DIFF LCG(LSFT(KC_D))
#define CX_BROWSER LSG(KC_L)
#define CX_TEXTBOX LSG(KC_A)
#define CX_COPYMODE LSG(KC_M)
#define CX_CLEAR LSG(KC_K)
#define CX_FLASH LSG(KC_H)
#define CX_ALLWIN LCAG(KC_DOT)

// agent keys: Claude Code cycles permission mode on Shift+Tab, interrupts on Esc
#define AI_MODE LSFT(KC_TAB)
#define AI_STOP KC_ESC
#define AI_KILL LCTL(KC_C)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* BASE (macOS)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  | CMUX |           | Del  |   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  | SYM  |           | SYM  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |Esc/VIM |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |   '    |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | Shift  |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | Shift  |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | Ctrl |  Alt |  Cmd | Left | Right|                                       |  Up  | Down |   [  |   ]  |  =   |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Alt  | Cmd  |       | Cmd  | Ctrl |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       | PgUp |      |      |
 *                                 | Space| Bksp |------|       |------| Tab  |Enter |
 *                                 |      |      | End  |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 */
[BASE] = LAYOUT_ergodox(
    KC_GRV,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    MO(CMUX),
    KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    MO(SYM),
    ESC_VIM,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
    KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_HYPR,
    KC_LCTL,  KC_LALT, KC_LGUI, KC_LEFT, KC_RGHT,
                                                  KC_LALT, KC_LGUI,
                                                           KC_HOME,
                                         KC_SPC,  KC_BSPC, KC_END,

    KC_DEL,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
    MO(SYM),  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_MEH,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                       KC_UP,   KC_DOWN, KC_LBRC, KC_RBRC, KC_EQL,
    KC_RGUI,  KC_RCTL,
    KC_PGUP,
    KC_PGDN,  KC_TAB,  KC_ENT
),

/* SYM (hold either inner 1.5u key on the QWERTY row)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  | F10  |  F11   |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   @  |   {  |   }  |   |  |      |           |      |      |   7  |   8  |   9  |   *  |  F12   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   #  |   $  |   (  |   )  |   `  |------|           |------|      |   4  |   5  |   6  |   +  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |   .  |   0  |   =  |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       | Mute |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       | Vol+ |      |      |
 *                                 |      |      |------|       |------| Play |      |
 *                                 |      |      |      |       | Vol- |      |      |
 *                                 `--------------------'       `--------------------'
 */
[SYM] = LAYOUT_ergodox(
    _______,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,
    _______,  KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE, _______,
    _______,  KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,
    _______,  KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, _______,
    _______,  _______, _______, _______, _______,
                                                  _______, _______,
                                                           _______,
                                         _______, _______, _______,

    _______,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    _______,  _______, KC_7,    KC_8,    KC_9,    KC_ASTR, KC_F12,
              _______, KC_4,    KC_5,    KC_6,    KC_PLUS, _______,
    _______,  KC_AMPR, KC_1,    KC_2,    KC_3,    KC_BSLS, _______,
                       _______, KC_DOT,  KC_0,    KC_EQL,  _______,
    KC_MUTE,  _______,
    KC_VOLU,
    KC_VOLD,  KC_MPLY, _______
),

/* VIM (hold the Esc key in the Caps Lock position; tap for Esc)
 *   Shift passes through, so Shift+H/J/K/L extends the selection like Shift+arrows.
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      | Line>|      |      |           |      |      |      |      |      |Line< |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      | Word>| Word>| Redo |      |      |           |      | Copy | PgUp |Line< |      |Paste |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |  (held)| Line>|      | PgDn | Find |DocTop|------|           |------| Left | Down |  Up  | Right|DocEnd|        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | Undo | Cut  | Copy |Paste | Word<|      |           |      |FindN |      |      |      | Find |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Home | End  |                                       | PgUp | PgDn |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      | Del  |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[VIM] = LAYOUT_ergodox(
    _______,  _______, _______, _______, LINE_R,  _______, _______,
    _______,  _______, WORD_R,  WORD_R,  REDO,    _______, _______,
    _______,  LINE_R,  _______, KC_PGDN, FIND,    DOC_TOP,
    _______,  UNDO,    CUT,     COPY,    PASTE,   WORD_L,  _______,
    _______,  _______, _______, KC_HOME, KC_END,
                                                  _______, _______,
                                                           _______,
                                         _______, KC_DEL,  _______,

    _______,  _______, _______, _______, _______, LINE_L,  _______,
    _______,  COPY,    KC_PGUP, LINE_L,  _______, PASTE,   _______,
              KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, DOC_END, _______,
    _______,  FIND_N,  _______, _______, _______, FIND,    _______,
                       KC_PGUP, KC_PGDN, _______, _______, _______,
    _______,  _______,
    _______,
    _______,  _______, _______
),

/* CMUX (hold the top inner 1.5u key on the left half)
 *   H/J/K/L focus panes, [ ] cycle surfaces, , . cycle workspaces, 1-9 jump to a workspace.
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | AllWin | WS 1 | WS 2 | WS 3 | WS 4 | WS 5 |(held)|           |      | WS 6 | WS 7 | WS 8 | WS 9 |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |Kill^C|CloseW|Equal |Rename|NewTab|      |           |      |Unread|      |Notifs|Browse|Palett|        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |TxtBox|SplitD|SplitR|Search| Diff |------|           |------|Pane< |Pane v|Pane ^|Pane> | Done |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | Zoom |Clear |CopyMd|      |Sidebr|      |           |      | NewWS| Mode |PrevWS|NextWS|Flash |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |Resz< |Resz> |                                       |Resz ^|Resz v| Surf<| Surf>|Reopen|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |GoToWS| Stop |------|       |------| Mode |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[CMUX] = LAYOUT_ergodox(
    CX_ALLWIN, LGUI(KC_1), LGUI(KC_2), LGUI(KC_3), LGUI(KC_4), LGUI(KC_5), _______,
    _______,   AI_KILL,    CX_WS_CLOSE, CX_EQUALIZE, CX_TAB_RENAME, CX_TAB_NEW, _______,
    _______,   CX_TEXTBOX, CX_SPLIT_D, CX_SPLIT_R, CX_SEARCH,  CX_DIFF,
    _______,   CX_ZOOM,    CX_CLEAR,   CX_COPYMODE, _______,   CX_SIDEBAR, _______,
    _______,   _______,    _______,    CX_RESIZE_L, CX_RESIZE_R,
                                                          _______, _______,
                                                                   _______,
                                              CX_WS_GO,   AI_STOP, _______,

    _______,   LGUI(KC_6), LGUI(KC_7), LGUI(KC_8), LGUI(KC_9), _______,   _______,
    _______,   CX_UNREAD,  _______,    CX_NOTIFS,  CX_BROWSER, CX_PALETTE, _______,
               CX_PANE_L,  CX_PANE_D,  CX_PANE_U,  CX_PANE_R,  CX_WS_DONE, _______,
    _______,   CX_WS_NEW,  AI_MODE,    CX_WS_P,    CX_WS_N,    CX_FLASH,  _______,
                           CX_RESIZE_U, CX_RESIZE_D, CX_SURF_P, CX_SURF_N, CX_TAB_REOPEN,
    _______,   _______,
    _______,
    _______,   AI_MODE,    _______
),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (get_highest_layer(state)) {
        case SYM:
            ergodox_right_led_1_on();
            break;
        case VIM:
            ergodox_right_led_2_on();
            break;
        case CMUX:
            ergodox_right_led_3_on();
            break;
    }
    return state;
}
