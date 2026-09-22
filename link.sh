#!/bin/sh
set -eu
QMK_HOME="${QMK_HOME:-$HOME/qmk_firmware}"
DEST="$QMK_HOME/keyboards/input_club/ergodox_infinity/keymaps/fd"
SRC="$(cd "$(dirname "$0")" && pwd)/keymaps/fd"
[ -e "$DEST" ] && [ ! -L "$DEST" ] && { echo "refusing to replace non-symlink $DEST" >&2; exit 1; }
ln -sfn "$SRC" "$DEST"
echo "linked $DEST -> $SRC"
