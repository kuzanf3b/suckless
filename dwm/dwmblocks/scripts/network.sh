#!/usr/bin/env bash

# Icons (Nerd Font)
ICON_WIFI_0=""
ICON_WIFI_25="󰤟"
ICON_WIFI_50="󰤢"
ICON_WIFI_75="󰤥"
ICON_WIFI_100="󰤨"
ICON_ETH=""

# Detect ethernet interface (non-wireless)
get_eth() {
    for if in /sys/class/net/*; do
        iface=$(basename "$if")
        [ "$iface" = "lo" ] && continue
        [ -d "/sys/class/net/$iface/wireless" ] && continue
        state=$(cat "/sys/class/net/$iface/operstate" 2>/dev/null)
        [ "$state" = "up" ] && echo "$iface" && return
    done
}

# Detect wifi interface (wireless + up)
get_wifi_iface() {
    for if in /sys/class/net/*; do
        iface=$(basename "$if")
        [ -d "/sys/class/net/$iface/wireless" ] || continue
        state=$(cat "/sys/class/net/$iface/operstate" 2>/dev/null)
        [ "$state" = "up" ] && echo "$iface" && return
    done
}

ethif=$(get_eth)
wiface=$(get_wifi_iface)

# --- Ethernet: prioritas tertinggi ---
if [ -n "$ethif" ]; then
    printf "%s %s\n" "$ICON_ETH" "$ethif"
    exit 0
fi

# --- WiFi ---
if [ -n "$wiface" ]; then
    ssid=$(iw dev "$wiface" link 2>/dev/null | awk -F': ' '/SSID/ {print $2}')

    # Sinyal
    sig=$(iw dev "$wiface" link | awk '/signal:/ {print $2}')

    if [[ "$sig" =~ ^-?[0-9]+$ ]]; then
        p=$(( 2 * (sig + 100) ))
        (( p < 0 )) && p=0
        (( p > 100 )) && p=100
    else
        p=0
    fi

    if   [ "$p" -ge 100 ]; then icon="$ICON_WIFI_100"
    elif [ "$p" -ge 75 ];  then icon="$ICON_WIFI_75"
    elif [ "$p" -ge 50 ];  then icon="$ICON_WIFI_50"
    elif [ "$p" -ge 25 ];  then icon="$ICON_WIFI_25"
    else                    icon="$ICON_WIFI_0"
    fi

    printf "%s %s\n" "$icon" "${ssid:-Unknown}"
    exit 0
fi

# --- Offline ---
printf "%s Offline\n" "$ICON_WIFI_0"
