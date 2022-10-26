#!/usr/bin/env bash

THEME="murz"

killall polybar
while pgrep -u $UID -x polybar >/dev/null; do sleep 1; done

CONFIG_DIR=$HOME/.config/polybar/$THEME/config.ini
polybar main -c $CONFIG_DIR &
