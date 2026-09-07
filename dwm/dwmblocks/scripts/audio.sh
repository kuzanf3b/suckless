#!/usr/bin/env bash

# Notifikasi sederhana
notify() {
    command -v notify-send >/dev/null && notify-send -u low -t 2000 "$1" "$2"
}

# --- Mouse Interaction ---
if [[ -n "$BLOCK_BUTTON" ]]; then
    case "$BLOCK_BUTTON" in
        1)  # Left click → toggle mute
            pamixer -t ;;
        2)  # Middle click → set volume 50%
            pamixer --set-volume 50 ;;
        3)  # Right click → show notification
            vol=$(pamixer --get-volume)
            mute=$(pamixer --get-mute)
            if [[ $mute == true ]]; then
                notify "Muted" "Audio is muted"
            else
                notify "Volume" "${vol}%"
            fi
            ;;
        4)  # Scroll up → volume up
            pamixer -i 5 ;;
        5)  # Scroll down → volume down
            pamixer -d 5 ;;
    esac
fi

# --- Output untuk dwmblocks ---
mute=$(pamixer --get-mute)
if [[ $mute == true ]]; then
    echo "mute"
else
    vol=$(pamixer --get-volume)
    echo "${vol}%"
fi
