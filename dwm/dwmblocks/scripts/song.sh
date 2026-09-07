#!/usr/bin/env bash

notify() {
    command -v notify-send >/dev/null && notify-send -u low "$1" "$2" -t 5000
}

music_bar() {
    local frames=("▁" "▂" "▃" "▄" "▅" "▆" "▇" "█")
    local idx=$((RANDOM % 8))
    echo "${frames[$idx]}${frames[$(((idx + 2) % 8))]}${frames[$(((idx + 4) % 8))]}"
}

if [[ -n "$BLOCK_BUTTON" ]]; then
    if command -v playerctl >/dev/null; then
        info=$(playerctl metadata --format "🎧 {{artist}} - {{title}}\n💿 Album: {{xesam:album}}\n⏯ Status: {{status}}" 2>/dev/null)
    elif command -v mpc >/dev/null; then
        info="$(mpc current)\n$(mpc status | head -n2)"
    else
        info="No active music player"
    fi
    [[ -n "$info" ]] && notify "󰎈 Now Playing" "$info"
    exit
fi

if command -v playerctl >/dev/null; then
    status=$(playerctl status 2>/dev/null)
    player_name=$(playerctl -l 2>/dev/null | grep -i spotify && echo "spotify")
    meta=$(playerctl metadata --format '{{artist}} - {{title}}' 2>/dev/null)

    if [[ -z $meta || $status == "Stopped" ]]; then
        echo ""
        exit
    fi

    [[ ${#meta} -gt 30 ]] && meta="${meta:0:27}…"

    if [[ $status == "Playing" ]]; then
        icon=""
    elif [[ $status == "Paused" ]]; then
        icon=""
    else
        icon=""
    fi

    [[ $player_name == "spotify" ]] && icon=" $icon"

    echo "$icon $meta $(music_bar)"

elif command -v mpc >/dev/null; then
    cur=$(mpc current)
    if [[ -z $cur ]]; then
        echo ""
    else
        [[ ${#cur} -gt 30 ]] && cur="${cur:0:27}…"
        echo " $cur $(music_bar)"
    fi

else
    echo ""
fi
