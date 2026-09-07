/* user and group to drop privileges to */
static const char *user = "zen";
static const char *group = "zen";

static const char *colorname[NUMCOLS] = {
    [INIT] = "black",    /* after initialization */
    [INPUT] = "#005577", /* during input */
    [FAILED] = "red",    /* wrong password */
    [CAPS] = "orange",   /* CapsLock on */
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    {"color0", STRING, &colorname[INIT]},
    {"color4", STRING, &colorname[INPUT]},
    {"color1", STRING, &colorname[FAILED]},
    {"color3", STRING, &colorname[CAPS]},
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 1;

/* default message */
static const char *message = "Locked at " __DATE__ " " __TIME__;

/* text color */
static const char *text_color = "#C0CAF5";

/* text size / font */
static const char *font_name = "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1";

/* time in seconds before the monitor shuts down */
static const int monitortime = 30;
