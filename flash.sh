#!/bin/sh
# Flash one half. Plug in only that half, press its program button, then run this.
set -eu
export PATH="/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:/opt/homebrew/opt/arm-none-eabi-binutils/bin:$PATH"
QMK_HOME="${QMK_HOME:-$HOME/qmk_firmware}"
cd "$QMK_HOME"
qmk flash -kb input_club/ergodox_infinity -km fd
