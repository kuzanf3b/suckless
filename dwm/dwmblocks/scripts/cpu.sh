#!/usr/bin/env bash

notify() {
  command -v notify-send >/dev/null && notify-send -u low "$1" "$2" -t 5000
}

statfile="/tmp/dwm_cpu_prev"

read -r _ user nice system idle iowait irq softirq steal _ < <(grep '^cpu ' /proc/stat)
total=$((user + nice + system + idle + iowait + irq + softirq + steal))

if [[ -f $statfile ]]; then
  read -r prev_total prev_idle <"$statfile"
else
  prev_total=$total
  prev_idle=$idle
fi

echo "$total $idle" >"$statfile"

totald=$((total - prev_total))
idled=$((idle - prev_idle))
usage=$(((1000 * (totald - idled) / totald + 5) / 10))

if ((usage > 75)); then
  icon="󰈸"
elif ((usage > 40)); then
  icon="⚡"
else
  icon=""
fi

if [[ -n "$BLOCK_BUTTON" ]]; then
  top_processes=$(ps -eo comm,pcpu --sort=-pcpu | awk 'NR==2,NR==4 {printf "%s: %s%%\n", $1, $2}')
  notify "CPU Usage: ${usage}%" "$top_processes"
  exit
fi

echo "$icon ${usage}%"
