#! /bin/bash
#
picom --experimental-backends --config ~/.config/picom.conf &

$HOME/.screenlayout/display.sh
# # # gentoo-pipewire-launcher &
pgrep -x clipit > /dev/null || clipit &
pgrep -x fcitx > /dev/null || fcitx &
# pgrep -x flameshot > /dev/null || flameshot &
xsetroot -cursor_name left_ptr
nitrogen --restore &
xset dpms 600 600 600

