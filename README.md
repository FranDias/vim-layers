# vim-layers

A QMK keymap for the Input Club / Massdrop **Infinity ErgoDox** with a modal vim engine, a cmux layer, and
per-layer screens and lights. Keymap for the **Infinity ErgoDox** (`input_club/ergodox_infinity` in QMK).
The stock Input Club KLL firmware is replaced by QMK; both halves are flashed with the same image.

## Layers

| Layer | How to reach it | What it does |
| --- | --- | --- |
| BASE | default | Standard ErgoDox QWERTY with macOS mods on the thumbs (Alt, Cmd left; Cmd, Ctrl right). The Caps Lock position is the vim mode key; a plain Esc sits on the tall inner key right of N. |
| SYM | tap to toggle or hold either inner 1.5u key on the QWERTY row | F-keys on the number row, brackets and symbols on the left, numpad on the right, media on the right thumb. |
| VIM | tap the Esc key in the Caps Lock position to enter normal mode (nothing is sent); tap it again to send Esc and return to insert; hold it for momentary normal mode. The tall inner key right of N is a plain Esc. | A modal vim engine built on macOS editing shortcuts, see below. |
| CMUX | tap to toggle or hold the top inner 1.5u key on the left half | Drives cmux with its default shortcuts: H/J/K/L focus panes, [ ] cycle surfaces, , . cycle workspaces, 1 to 9 jump to a workspace, D/S split, T new tab, W close workspace, Z zoom, B sidebar, P palette, N new workspace. Left big thumb key opens the workspace switcher. M and the right inner big thumb key send Shift+Tab (Claude Code mode cycle), the left inner big thumb key sends Esc (interrupt the agent), Q sends Ctrl+C. |

### Vim engine

Normal mode interprets letters, digits and `/`; everything else (thumbs, mods, arrows) passes through.

| Group | Keys |
| --- | --- |
| motions | `h j k l`, `w e` word right, `b` word left, `0 ^ $`, `gg G`, any motion takes a count: `23e`, `10j` |
| insert | `i`, `a` (after cursor), `I` (line start), `A` (line end), `o O` (open a line), `s S`, `c C`, `/` (opens find) |
| edit | `x X`, `d{motion}` `dd` `D`, `y{motion}` `yy` `Y`, `p`, `u` undo, `U` or Ctrl+r redo, `J` join |
| visual | `v`, `V` (whole lines); motions extend, `d x y c p` act on the selection, Esc returns to normal |
| objects | `iw` after an operator or in visual selects the word under the cursor |
| paging | Ctrl+d Ctrl+u Ctrl+f Ctrl+b |

Deletes go through Cmd+X and yanks through Cmd+C, so `p` pastes the system clipboard. Not implemented: `.` repeat, registers, `r`, `%`, `*`, text objects other than `iw`, and `:` commands.

### Screens and lights

- The half on USB shows the active layer, a one-line cheat sheet, WPM and Caps.
- The other half shows the vim mode (`INSERT`, `NORMAL`, `VISUAL`, `V-LINE`) and the command typed so far, for example `23d`. That state is synced across the link.
- LCD backlight: white on BASE, blue on SYM, green in vim normal, amber in visual, orange on CMUX.
- Per-key backlight (white only on this board) lights the keys that do something on the active layer.

## Build

```sh
brew install qmk/qmk/qmk        # once; also `qmk setup -y -H ~/qmk_firmware`
./build.sh                      # links keymaps/fd into ~/qmk_firmware and compiles
```

Homebrew installs the ARM toolchain keg-only, so `build.sh` and `flash.sh` put it on PATH themselves.

A rendered view of every layer is in `keymap-viewer.html`; open it in a browser.

## Flash

Each half has its own MCU, so flash them one at a time with the same image. Plug the **left** half into USB
first: without `EE_HANDS`, QMK treats whichever half is on USB as the left one.

1. Connect only the left half. Press the program button through the hole on the underside, above the thumb cluster.
2. `./flash.sh`
3. Connect only the right half, press its program button, run the same command.
4. Reconnect the halves and plug USB into the left half.

`configurator.json` is an export for https://config.qmk.fm (keyboard `input_club/ergodox_infinity`). It cannot express the vim engine, the screens or the lights, so treat it as a viewer, not the source of truth. Keep it out of `keymaps/fd/`, where QMK would compile it instead of the C file.

To go back to the stock firmware, flash a build from the Input Club configurator the same way.

## Licence

GPL-2.0-or-later, the same as QMK.
