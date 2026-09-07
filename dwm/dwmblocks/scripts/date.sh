#!/usr/bin/env bash

notify() {
    command -v notify-send >/dev/null && notify-send -u low "$1" "$2" -t 5000
}

if [[ -n "$BLOCK_BUTTON" ]]; then
    notify "󰃭 Tanggal & Waktu" "$(date '+%A, %d %B %Y\n⏰ %I:%M %p')"
    exit
fi

echo "󰥔 $(date '+%a, %I:%M %p')"
