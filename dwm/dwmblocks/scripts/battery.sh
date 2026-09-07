#!/usr/bin/env bash

LOW_BATTERY=20
CRITICAL_BATTERY=10
NOTIFY_FILE="/tmp/battery_notified"

FG="#e0def4"
BG="#26233a"
ACCENT_LOW="#f6c177"
ACCENT_CRIT="#eb6f92"
ACCENT_OK="#9ccfd8"

notify() {
  local title=$1
  local message=$2
  local urgency=${3:-low}
  command -v notify-send >/dev/null && \
  notify-send -u "$urgency" -t 5000 \
    -h string:fgcolor:"$FG" \
    -h string:bgcolor:"$BG" \
    "$title" "$message"
}

get_battery() {
  if command -v upower >/dev/null; then
    BAT_PATH=$(upower -e | grep BAT)
    perc=$(upower -i "$BAT_PATH" | awk -F: '/percentage/ {gsub(/ /, "", $2); print $2; exit}')
    state=$(upower -i "$BAT_PATH" | awk -F: '/state/ {gsub(/ /, "", $2); print $2; exit}')
  elif command -v acpi >/dev/null; then
    perc=$(acpi -b | awk -F', ' '{print $2; exit}')
    state=$(acpi -b | awk -F', ' '{gsub(/ /,"",$1); print $1; exit}')
  else
    perc="$(cat /sys/class/power_supply/BAT*/capacity 2>/dev/null)%"
    state="unknown"
  fi
  perc_num=${perc//%/}
}

get_battery

# === Klik untuk detail ===
if [[ -n "$BLOCK_BUTTON" ]]; then
  if command -v upower >/dev/null; then
    info=$(upower -i "$BAT_PATH" | grep -E 'state|percentage|time')
  elif command -v acpi >/dev/null; then
    info=$(acpi -b)
  else
    info="${perc:-N/A}"
  fi
  notify "󰂄 Battery Info" "$info"
  exit
fi

# === Fungsi progress bar ===
progress_bar() {
  local width=10
  local fill=$((perc_num * width / 100))
  local bar=""
  for ((i = 0; i < width; i++)); do
    if ((i < fill)); then
      bar+="█"
    else
      bar+="░"
    fi
  done
  echo "$bar"
}

# === Notifikasi ===
if [[ "$state" != "charging" ]]; then
  if [[ "$perc_num" -le $CRITICAL_BATTERY ]]; then
    [[ ! -f "$NOTIFY_FILE" || "$(cat $NOTIFY_FILE)" != "critical" ]] && {
      notify "󰂎 Battery Critical" "$(progress_bar) $perc – Plug in now!" critical
      echo "critical" > "$NOTIFY_FILE"
    }
  elif [[ "$perc_num" -le $LOW_BATTERY ]]; then
    [[ ! -f "$NOTIFY_FILE" || "$(cat $NOTIFY_FILE)" != "low" ]] && {
      notify "󰁻 Battery Low" "$(progress_bar) $perc – Consider charging." normal
      echo "low" > "$NOTIFY_FILE"
    }
  else
    rm -f "$NOTIFY_FILE" 2>/dev/null
  fi
fi

# === Ikon ===
if [[ "$state" == "charging" ]] || [[ "$state" == "fully-charged" ]]; then
  if   [[ "$perc_num" -le 10 ]]; then icon="󰢜"
  elif [[ "$perc_num" -le 20 ]]; then icon="󰂆"
  elif [[ "$perc_num" -le 30 ]]; then icon="󰂇"
  elif [[ "$perc_num" -le 40 ]]; then icon="󰂈"
  elif [[ "$perc_num" -le 50 ]]; then icon="󰢝"
  elif [[ "$perc_num" -le 60 ]]; then icon="󰂉"
  elif [[ "$perc_num" -le 70 ]]; then icon="󰢞"
  elif [[ "$perc_num" -le 80 ]]; then icon="󰂊"
  elif [[ "$perc_num" -le 90 ]]; then icon="󰂋"
  else icon="󰂅"
  fi
else
  if   [[ "$perc_num" -le 5 ]];  then icon="󰂎"
  elif [[ "$perc_num" -le 10 ]]; then icon="󰁺"
  elif [[ "$perc_num" -le 20 ]]; then icon="󰁻"
  elif [[ "$perc_num" -le 30 ]]; then icon="󰁼"
  elif [[ "$perc_num" -le 40 ]]; then icon="󰁽"
  elif [[ "$perc_num" -le 50 ]]; then icon="󰁾"
  elif [[ "$perc_num" -le 60 ]]; then icon="󰁿"
  elif [[ "$perc_num" -le 70 ]]; then icon="󰂀"
  elif [[ "$perc_num" -le 80 ]]; then icon="󰂁"
  elif [[ "$perc_num" -le 90 ]]; then icon="󰂂"
  else icon="󰁹"
  fi
fi

echo "$icon $perc"
