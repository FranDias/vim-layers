#!/bin/sh
# Homebrew installs the QMK toolchains keg-only, so they are not on PATH by default.
set -eu
export PATH="/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:/opt/homebrew/opt/arm-none-eabi-binutils/bin:$PATH"
QMK_HOME="${QMK_HOME:-$HOME/qmk_firmware}"
cd "$QMK_HOME"
[ -L keyboards/input_club/ergodox_infinity/keymaps/fd ] || "$(dirname "$0")/link.sh"
qmk compile -kb input_club/ergodox_infinity -km fd
