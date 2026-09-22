#include QMK_KEYBOARD_H
#include <string.h>
#include "transactions.h"

enum layers {
    BASE,
    SYM,
    VIM,
    CMUX,
};

enum custom_keycodes {
    VIM_ESC = SAFE_RANGE,
};

enum vim_mode {
    VM_INSERT,
    VM_NORMAL,
    VM_VISUAL,
    VM_VLINE,
};

typedef struct {
    uint8_t  mode;
    uint16_t count;
    uint8_t  op;      // 0, 'd', 'c' or 'y'
    uint8_t  pending; // 0, 'g' (gg) or 'i' (text object)
} vim_state_t;

static vim_state_t vim;        // authoritative on the USB half
static vim_state_t vim_remote; // mirrored to the other half for its display

#define VIM_HOLD_TERM 200
#define VIM_MAX_COUNT 999

// cmux default shortcuts (https://cmux.com/docs/keyboard-shortcuts)
#define CX_PANE_L LAG(KC_LEFT)
#define CX_PANE_D LAG(KC_DOWN)
#define CX_PANE_U LAG(KC_UP)
#define CX_PANE_R LAG(KC_RGHT)
#define CX_SURF_P LSG(KC_LBRC)
#define CX_SURF_N LSG(KC_RBRC)
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
 * |--------+------+------+------+------+------| Hyper|           | Esc  |------+------+------+------+------+--------|
 * | Shift  |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | Shift  |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | Ctrl |  Alt |  Cmd | Left | Right|                                       |  Up  | Down |   [  |   ]  |  =   |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Alt  | Cmd  |       | Cmd  | Ctrl |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       | PgUp |      |      |
 *                                 | Bksp | Tab  |------|       |------|Enter |Space |
 *                                 |      |      | End  |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 *
 * CMUX and SYM: tap to toggle, hold for momentary. Esc/VIM: tap enters normal mode, tap again sends Esc and returns
 * to insert, hold for momentary normal mode. The tall key right of N is a plain Esc.
 */
[BASE] = LAYOUT_ergodox(
    KC_GRV,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    TT(CMUX),
    KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    TT(SYM),
    VIM_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
    KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_HYPR,
    KC_LCTL,  KC_LALT, KC_LGUI, KC_LEFT, KC_RGHT,
                                                  KC_LALT, KC_LGUI,
                                                           KC_HOME,
                                         KC_BSPC, KC_TAB,  KC_END,

    KC_DEL,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
    TT(SYM),  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_ESC,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                       KC_UP,   KC_DOWN, KC_LBRC, KC_RBRC, KC_EQL,
    KC_RGUI,  KC_RCTL,
    KC_PGUP,
    KC_PGDN,  KC_ENT,  KC_SPC
),

/* SYM (tap or hold either inner 1.5u key on the QWERTY row)
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

/* VIM: every key falls through to BASE; vim_process() below interprets letters, digits and / as vim commands.
 *
 *   motions   h j k l   w e (word right)   b (word left)   0 ^ $   gg G   [count]motion
 *   insert    i  a (after)  I (line start)  A (line end)  o O (open line)  s S  c C
 *   edit      x X   d{motion} dd D   y{motion} yy Y   p   u (undo) U / ctrl-r (redo)   J (join)
 *   visual    v V   then motions extend, d x y c p act on the selection, Esc drops back to normal
 *   objects   iw after an operator or in visual selects the word under the cursor
 *   search    / (opens find and drops to insert)   n N
 *   paging    ctrl-d ctrl-u ctrl-f ctrl-b
 */
[VIM] = LAYOUT_ergodox(
    _______,  _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______,
                                                  _______, _______,
                                                           _______,
                                         _______, _______, _______,

    _______,  _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______, _______, _______,
              _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______, _______, _______,
                       _______, _______, _______, _______, _______,
    _______,  _______,
    _______,
    _______,  _______, _______
),

/* CMUX (tap or hold the top inner 1.5u key on the left half)
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

/* ---------------------------------------------------------------- vim engine */

static void tap_n(uint16_t kc, uint16_t n) {
    for (uint16_t i = 0; i < n; i++) {
        tap_code16(kc);
    }
}

static uint16_t sel(uint16_t kc, bool select) {
    return select ? (kc | QK_LSFT) : kc;
}

static void vim_reset_pending(void) {
    vim.count   = 0;
    vim.op      = 0;
    vim.pending = 0;
}

static void vim_set_mode(uint8_t mode) {
    vim.mode = mode;
    vim_reset_pending();
    if (mode == VM_INSERT) {
        layer_off(VIM);
    } else {
        layer_on(VIM);
    }
}

static bool vim_handles(uint16_t kc) {
    return (kc >= KC_A && kc <= KC_0) || kc == KC_SLSH;
}

// Moves (or extends the selection when select is set). Returns the direction moved, 0 when kc is not a motion.
static int8_t vim_motion(uint16_t kc, bool shift, uint16_t n, bool select) {
    switch (kc) {
        case KC_H: tap_n(sel(KC_LEFT, select), n); return -1;
        case KC_J: tap_n(sel(KC_DOWN, select), n); return 1;
        case KC_K: tap_n(sel(KC_UP, select), n); return -1;
        case KC_L: tap_n(sel(KC_RGHT, select), n); return 1;
        case KC_W:
        case KC_E: tap_n(sel(LALT(KC_RGHT), select), n); return 1;
        case KC_B: tap_n(sel(LALT(KC_LEFT), select), n); return -1;
        case KC_0: tap_code16(sel(LGUI(KC_LEFT), select)); return -1;
        case KC_4: if (!shift) return 0; tap_code16(sel(LGUI(KC_RGHT), select)); return 1;  // $
        case KC_6: if (!shift) return 0; tap_code16(sel(LGUI(KC_LEFT), select)); return -1; // ^
        case KC_G: if (!shift) return 0; tap_code16(sel(LGUI(KC_DOWN), select)); return 1;  // G
    }
    return 0;
}

// Runs the pending operator on a selection that was just made in direction dir.
static void vim_apply_op(int8_t dir) {
    switch (vim.op) {
        case 'd':
            tap_code16(LGUI(KC_X));
            vim_reset_pending();
            break;
        case 'c':
            tap_code16(LGUI(KC_X));
            vim_set_mode(VM_INSERT);
            break;
        case 'y':
            tap_code16(LGUI(KC_C));
            tap_code(dir > 0 ? KC_LEFT : KC_RGHT);
            vim_reset_pending();
            break;
        default:
            vim_reset_pending();
    }
}

// dd / yy / cc and their shifted forms, over n lines.
static void vim_line_op(uint8_t op, uint16_t n) {
    tap_code16(LGUI(KC_LEFT));
    if (op == 'c') {
        tap_n(S(KC_DOWN), n - 1);
        tap_code16(LSG(KC_RGHT));
        tap_code16(LGUI(KC_X));
        vim_set_mode(VM_INSERT);
        return;
    }
    tap_n(S(KC_DOWN), n);
    tap_code16(op == 'd' ? LGUI(KC_X) : LGUI(KC_C));
    if (op == 'y') {
        tap_code(KC_LEFT);
    }
    vim_reset_pending();
}

// iw: select the word under the cursor, then act on it.
static void vim_inner_word(void) {
    tap_code16(LALT(KC_LEFT));
    tap_code16(LSA(KC_RGHT));
    if (vim.op) {
        vim_apply_op(1);
    } else {
        vim.pending = 0;
    }
}

static void vim_visual_command(uint16_t kc, bool shift, uint16_t n) {
    bool line = vim.mode == VM_VLINE;
    int8_t dir = vim_motion(kc, shift, n, true);
    if (dir) {
        if (line && kc == KC_J) tap_code16(LSG(KC_RGHT));
        if (line && kc == KC_K) tap_code16(LSG(KC_LEFT));
        vim.count = 0;
        return;
    }
    switch (kc) {
        case KC_D:
        case KC_X:
            tap_code16(LGUI(KC_X));
            vim_set_mode(VM_NORMAL);
            break;
        case KC_Y:
            tap_code16(LGUI(KC_C));
            tap_code(KC_LEFT);
            vim_set_mode(VM_NORMAL);
            break;
        case KC_C:
        case KC_S:
            tap_code16(LGUI(KC_X));
            vim_set_mode(VM_INSERT);
            break;
        case KC_P:
            tap_code16(LGUI(KC_V));
            vim_set_mode(VM_NORMAL);
            break;
        case KC_V:
            tap_code(KC_RGHT);
            vim_set_mode(VM_NORMAL);
            break;
        case KC_I:
            vim.pending = 'i';
            break;
        case KC_G:
            if (!shift) vim.pending = 'g';
            break;
        case KC_U:
            tap_code16(LGUI(KC_Z));
            vim_set_mode(VM_NORMAL);
            break;
    }
}

static void vim_normal_command(uint16_t kc, bool shift, uint16_t n) {
    if (vim.op) {
        if ((kc == KC_D && vim.op == 'd') || (kc == KC_C && vim.op == 'c') || (kc == KC_Y && vim.op == 'y')) {
            vim_line_op(vim.op, n);
            return;
        }
        if (kc == KC_I) { vim.pending = 'i'; return; }
        if (kc == KC_G && !shift) { vim.pending = 'g'; return; }
        int8_t dir = vim_motion(kc, shift, n, true);
        if (dir) {
            vim_apply_op(dir);
        } else {
            vim_reset_pending();
        }
        return;
    }

    if (vim_motion(kc, shift, n, false)) {
        vim.count = 0;
        return;
    }

    switch (kc) {
        case KC_G: vim.pending = 'g'; return;
        case KC_I:
            if (shift) tap_code16(LGUI(KC_LEFT));
            vim_set_mode(VM_INSERT);
            return;
        case KC_A:
            tap_code16(shift ? LGUI(KC_RGHT) : KC_RGHT);
            vim_set_mode(VM_INSERT);
            return;
        case KC_O:
            if (shift) {
                tap_code16(LGUI(KC_LEFT));
                tap_code(KC_ENT);
                tap_code(KC_UP);
            } else {
                tap_code16(LGUI(KC_RGHT));
                tap_code(KC_ENT);
            }
            vim_set_mode(VM_INSERT);
            return;
        case KC_D:
            if (shift) { tap_code16(LSG(KC_RGHT)); tap_code16(LGUI(KC_X)); vim_reset_pending(); }
            else vim.op = 'd';
            return;
        case KC_C:
            if (shift) { tap_code16(LSG(KC_RGHT)); tap_code16(LGUI(KC_X)); vim_set_mode(VM_INSERT); }
            else vim.op = 'c';
            return;
        case KC_Y:
            if (shift) vim_line_op('y', n);
            else vim.op = 'y';
            return;
        case KC_S:
            if (shift) { vim_line_op('c', n); }
            else { tap_n(KC_DEL, n); vim_set_mode(VM_INSERT); }
            return;
        case KC_X:
            tap_n(shift ? KC_BSPC : KC_DEL, n);
            vim_reset_pending();
            return;
        case KC_P:
            tap_n(LGUI(KC_V), n);
            vim_reset_pending();
            return;
        case KC_U:
            tap_n(shift ? LSG(KC_Z) : LGUI(KC_Z), n);
            vim_reset_pending();
            return;
        case KC_J: // only reached with shift: join lines
            for (uint16_t i = 0; i < n; i++) {
                tap_code16(LGUI(KC_RGHT));
                tap_code(KC_DEL);
                tap_code(KC_SPC);
            }
            vim_reset_pending();
            return;
        case KC_V:
            if (shift) {
                tap_code16(LGUI(KC_LEFT));
                tap_code16(LSG(KC_RGHT));
                vim_set_mode(VM_VLINE);
            } else {
                vim_set_mode(VM_VISUAL);
            }
            return;
        case KC_SLSH:
            tap_code16(LGUI(KC_F));
            vim_set_mode(VM_INSERT);
            return;
        case KC_N:
            tap_n(shift ? LSG(KC_G) : LGUI(KC_G), n);
            vim_reset_pending();
            return;
    }
    vim_reset_pending();
}

static bool vim_process(uint16_t kc, keyrecord_t *record) {
    if (!vim_handles(kc)) return true;
    uint8_t mods = get_mods();
    if (mods & (MOD_MASK_GUI | MOD_MASK_ALT)) return true;
    if (!record->event.pressed) return false;

    bool shift = mods & MOD_MASK_SHIFT;
    bool ctrl  = mods & MOD_MASK_CTRL;
    bool select = vim.mode == VM_VISUAL || vim.mode == VM_VLINE;
    uint16_t n = vim.count ? vim.count : 1;

    del_mods(MOD_MASK_SHIFT | MOD_MASK_CTRL);
    clear_weak_mods();

    if (vim.pending == 'g') {
        vim.pending = 0;
        if (kc == KC_G && !shift) {
            tap_code16(sel(LGUI(KC_UP), select || vim.op));
            if (vim.op) vim_apply_op(-1); else vim.count = 0;
            goto done;
        }
    }
    if (vim.pending == 'i') {
        vim.pending = 0;
        if (kc == KC_W) { vim_inner_word(); goto done; }
    }

    if (kc >= KC_1 && kc <= KC_9) {
        vim.count = MIN(vim.count * 10 + (kc - KC_1 + 1), VIM_MAX_COUNT);
        goto done;
    }
    if (kc == KC_0 && vim.count) {
        vim.count = MIN(vim.count * 10, VIM_MAX_COUNT);
        goto done;
    }

    if (ctrl) {
        switch (kc) {
            case KC_D: case KC_F: tap_n(sel(KC_PGDN, select), n); break;
            case KC_U: case KC_B: tap_n(sel(KC_PGUP, select), n); break;
            case KC_R: tap_n(LSG(KC_Z), n); break;
        }
        vim.count = 0;
        goto done;
    }

    if (select) {
        vim_visual_command(kc, shift, n);
    } else {
        vim_normal_command(kc, shift, n);
    }

done:
    set_mods(mods);
    return false;
}

// Tap from insert: normal mode, nothing sent. Tap from normal: send Esc, back to insert.
// Tap from visual: collapse to normal. Hold from insert: momentary normal mode.
static bool process_vim_esc(keyrecord_t *record) {
    static uint16_t held_since;
    static uint8_t  mode_at_press;
    if (record->event.pressed) {
        held_since    = timer_read();
        mode_at_press = vim.mode;
        if (mode_at_press == VM_INSERT) {
            vim_set_mode(VM_NORMAL);
        }
        return false;
    }
    switch (mode_at_press) {
        case VM_INSERT:
            if (timer_elapsed(held_since) >= VIM_HOLD_TERM) {
                vim_set_mode(VM_INSERT);
            }
            break;
        case VM_VISUAL:
        case VM_VLINE:
            tap_code(KC_RGHT);
            vim_set_mode(VM_NORMAL);
            break;
        default:
            tap_code(KC_ESC);
            vim_set_mode(VM_INSERT);
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == VIM_ESC) {
        return process_vim_esc(record);
    }
    if (vim.mode != VM_INSERT && get_highest_layer(layer_state) == VIM) {
        return vim_process(keycode, record);
    }
    return true;
}

/* ------------------------------------------------------- split sync of vim state */

static void vim_sync_slave_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
    if (in_len == sizeof(vim_state_t)) {
        memcpy(&vim_remote, in_data, sizeof(vim_state_t));
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_VIM, vim_sync_slave_handler);
#ifdef LED_MATRIX_ENABLE
    led_matrix_mode_noeeprom(LED_MATRIX_SOLID);
    led_matrix_set_val_noeeprom(LED_MATRIX_DEFAULT_VAL);
#endif
}

void housekeeping_task_user(void) {
    if (!is_keyboard_master()) return;
    static vim_state_t sent;
    static uint32_t    last;
    if (timer_elapsed32(last) < 50) return;
    if (memcmp(&sent, &vim, sizeof(vim_state_t)) == 0) return;
    if (transaction_rpc_send(USER_SYNC_VIM, sizeof(vim_state_t), &vim)) {
        sent = vim;
        last = timer_read32();
    }
}

static const vim_state_t *vim_view(void) {
    return is_keyboard_master() ? &vim : &vim_remote;
}

/* --------------------------------------------------------- LCD backlight colour */

static void lcd_color_for_state(void) {
    const vim_state_t *v = vim_view();
    switch (get_highest_layer(layer_state)) {
        case CMUX: ergodox_infinity_lcd_color(UINT16_MAX, UINT16_MAX / 5, 0); return;             // orange
        case SYM:  ergodox_infinity_lcd_color(0, UINT16_MAX / 3, UINT16_MAX); return;              // blue
        case VIM:
            if (v->mode == VM_VISUAL || v->mode == VM_VLINE) {
                ergodox_infinity_lcd_color(UINT16_MAX, UINT16_MAX / 2, 0);                         // amber
            } else {
                ergodox_infinity_lcd_color(0, UINT16_MAX, UINT16_MAX / 6);                         // green
            }
            return;
        default:   ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, UINT16_MAX / 2);    // white
    }
}

#ifdef ST7565_ENABLE
void st7565_on_user(void) {
    lcd_color_for_state();
}

static const char *layer_name(uint8_t layer) {
    switch (layer) {
        case SYM:  return "SYM";
        case VIM:  return "VIM";
        case CMUX: return "CMUX";
        default:   return "BASE";
    }
}

static const char *layer_hint(uint8_t layer) {
    switch (layer) {
        case SYM:  return "F-keys  sym  numpad";
        case VIM:  return "hjkl wbe 0$ dcy v iw";
        case CMUX: return "hjkl pane []tab ,.ws";
        default:   return "";
    }
}

static const char *mode_name(uint8_t mode) {
    switch (mode) {
        case VM_NORMAL: return "-- NORMAL --";
        case VM_VISUAL: return "-- VISUAL --";
        case VM_VLINE:  return "-- V-LINE --";
        default:        return "-- INSERT --";
    }
}

static void vim_pending_string(const vim_state_t *v, char *buf, size_t len) {
    size_t i = 0;
    if (v->count) {
        i += snprintf(buf, len, "%u", v->count);
    }
    if (v->op && i < len - 1) buf[i++] = v->op;
    if (v->pending && i < len - 1) buf[i++] = v->pending;
    buf[i] = 0;
}

void st7565_task_user(void) {
    lcd_color_for_state();
    uint8_t layer = get_highest_layer(layer_state);
    st7565_clear();
    if (is_keyboard_master()) {
        char line[22];
        led_t leds = host_keyboard_led_state();
        snprintf(line, sizeof(line), "layer  %-6s %s", layer_name(layer), leds.caps_lock ? "CAPS" : "");
        st7565_set_cursor(0, 0);
        st7565_write_ln(line, false);
        st7565_set_cursor(0, 1);
        st7565_write_ln(layer_hint(layer), false);
        snprintf(line, sizeof(line), "wpm    %u", get_current_wpm());
        st7565_set_cursor(0, 2);
        st7565_write_ln(line, false);
    } else {
        const vim_state_t *v = vim_view();
        char pending[16];
        vim_pending_string(v, pending, sizeof(pending));
        st7565_set_cursor(0, 0);
        st7565_write_ln(mode_name(v->mode), false);
        st7565_set_cursor(0, 1);
        st7565_write_ln(pending, false);
        st7565_set_cursor(0, 2);
        st7565_write_ln(layer == VIM ? "" : layer_name(layer), false);
    }
}
#endif

/* -------------------------------------------------- per-key backlight by layer */

#ifdef LED_MATRIX_ENABLE
static bool key_active_on_layer(uint8_t layer, uint8_t row, uint8_t col) {
    keypos_t pos  = {.row = row, .col = col};
    uint16_t base = keymap_key_to_keycode(BASE, pos);
    if (base == TT(layer) || base == VIM_ESC) return true;
    if (layer == VIM) return vim_handles(base);
    return keymap_key_to_keycode(layer, pos) != KC_TRNS;
}

bool led_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer == BASE) return true;
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED || led < led_min || led >= led_max) continue;
            led_matrix_set_value(led, key_active_on_layer(layer, row, col) ? 255 : 0);
        }
    }
    return true;
}
#endif
