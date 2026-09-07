// Modify this file to change what commands output to your statusbar, and
// recompile using the make command.

static const Block blocks[] = {
    /* Icon | Command | Interval | Signal */
    {"", "/home/kuzan/Repository/dwm-dotfiles/dwmblocks/scripts/song.sh", 1, 5},
    {"", "/home/kuzan/Repository/dwm-dotfiles/dwmblocks/scripts/network.sh", 1, 3},
    {"", "/home/kuzan/Repository/dwm-dotfiles/dwmblocks/scripts/battery.sh", 3, 2},
    {"", "/home/kuzan/Repository/dwm-dotfiles/dwmblocks/scripts/audio.sh", 1, 4},
    {"", "/home/kuzan/Repository/dwm-dotfiles/dwmblocks/scripts/date.sh", 60, 1},
};

// sets delimiter between status commands.
// NULL character ('\0') means no delimiter.
static char delim[] = " | ";
static unsigned int delimLen = 3;
