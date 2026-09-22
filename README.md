# ErgoDox Infinity keymap (QMK)

Keymap for the Input Club / Massdrop **Infinity ErgoDox** (`input_club/ergodox_infinity` in QMK).
The stock Input Club KLL firmware is replaced by QMK; both halves are flashed with the same image.

## Layers

| Layer | How to reach it | What it does |
| --- | --- | --- |
| BASE | default | Standard ErgoDox QWERTY with macOS mods on the thumbs (Alt, Cmd left; Cmd, Ctrl right). Esc sits in the Caps Lock position. |
| SYM | hold either inner 1.5u key on the QWERTY row | F-keys on the number row, brackets and symbols on the left, numpad on the right, media on the right thumb. |
| VIM | hold Esc (Caps Lock position); tap for Esc | H/J/K/L arrows, W/E and B word jumps, I/A and 0/4 line start/end, G and ; document top/end, U/D page up/down, Z/R undo/redo, X/C/V cut/copy/paste, F and / find, N find next. Shift passes through, so Shift+J extends a selection. |
| CMUX | hold the top inner 1.5u key on the left half | Drives cmux: H/J/K/L focus panes, [ ] cycle surfaces, , . cycle workspaces, 1 to 9 jump to a workspace, D/S split, T new tab, W close workspace, Z zoom, B sidebar, P palette, N new workspace. Left big thumb key opens the workspace switcher. M and the right inner big thumb key send Shift+Tab (Claude Code mode cycle), the left inner big thumb key sends Esc (interrupt the agent), Q sends Ctrl+C. |

The right-hand LEDs show the active layer: 1 = SYM, 2 = VIM, 3 = CMUX.

Every cmux key sends the cmux default shortcut, so `~/.config/cmux/cmux.json` must not rebind them. If you change a cmux binding, update the matching `CX_*` define in `keymaps/fd/keymap.c`.

## Build

```sh
brew install qmk/qmk/qmk        # once; also `qmk setup -y -H ~/qmk_firmware`
./build.sh                      # links keymaps/fd into ~/qmk_firmware and compiles
```

Homebrew installs the ARM toolchain keg-only, so `build.sh` and `flash.sh` put it on PATH themselves.

## Flash

Each half has its own MCU, so flash them one at a time with the same image. Plug the **left** half into USB
first: without `EE_HANDS`, QMK treats whichever half is on USB as the left one.

1. Connect only the left half. Press the program button through the hole on the underside, above the thumb cluster.
2. `./flash.sh`
3. Connect only the right half, press its program button, run the same command.
4. Reconnect the halves and plug USB into the left half.

To go back to the stock firmware, flash a build from the Input Club configurator the same way.
