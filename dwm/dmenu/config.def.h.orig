/* Default settings; can be overridden by command line. */

/* ── Layout ────────────────────────────────────────────── */
static int topbar = 1;       /* -b : 0 = bottom, 1 = top               */
static int horizpadbar = 8; /* horizontal padding                     */
static int vertpadbar = 12;  /* vertical padding                       */
static int centered = 1;     /* -c : center the menu                   */
static int min_width = 500;  /* minimum width when centered            */
static const float menu_height_ratio = 4.0f;

/* ── Opacity ───────────────────────────────────────────── */
static const unsigned int alpha = 0x99; /* 0xff = fully opaque */

/* ── Fonts ─────────────────────────────────────────────── */
static char font[] = "monospace:size=10";
static const char *fonts[] = {font, "monospace:size=12"};

/* ── Prompt ────────────────────────────────────────────── */
static char *prompt = NULL; /* -p : prompt text left of input field  */

/* ── Fuzzymatch ────────────────────────────────────────── */
static int fuzzy = 1; /* -F : enable fuzzy-matching by default */

/* ── Default Colors ────────────────────────────────────── */
static char normfgcolor[] = "#bbbbbb";
static char normbgcolor[] = "#222222";
static char selfgcolor[] = "#eeeeee";
static char selbgcolor[] = "#005577";
static char bordercolor[] = "#7aa2f7"; /* border color for dmenu */

/* ── Fuzzyhighlight Colors ─────────────────────────────── */
static char selhighlightfg[] = "#ffc978";
static char selhighlightbg[] = "#005577";
static char normhighlightfg[] = "#ffc978";
static char normhighlightbg[] = "#222222";

/* ── Color Schemes ─────────────────────────────────────── */
static const char *colors[SchemeLast][3] = {
    /*                 fg                  bg               border     */
    [SchemeNorm] = {normfgcolor, normbgcolor, bordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, bordercolor},
    [SchemeNormHighlight] = {normhighlightfg, normhighlightbg, bordercolor},
    [SchemeSelHighlight] = {selhighlightfg, selhighlightbg, bordercolor},
    [SchemeOut] = {"#000000", "#00ffff", bordercolor},
};

/* ── Opacity per Scheme ────────────────────────────────── */
static const unsigned int alphas[SchemeLast][2] = {
    [SchemeNorm] = {OPAQUE, alpha},
    [SchemeSel] = {OPAQUE, alpha},
    [SchemeOut] = {OPAQUE, alpha},
};

/* ── List Layout ───────────────────────────────────────── */
static unsigned int lines = 5;        /* -l : vertical list line count       */
static unsigned int maxhist    = 64;
static unsigned int border_width = 2; /* default border thickness            */

/* ── Xresources Preferences ────────────────────────────── */
ResourcePref resources[] = {
    {"font", STRING, &font},
    {"normfgcolor", STRING, &normfgcolor},
    {"normbgcolor", STRING, &normbgcolor},
    {"selfgcolor", STRING, &selfgcolor},
    {"selbgcolor", STRING, &selbgcolor},
    {"selhighlightfg", STRING, &selhighlightfg},
    {"selhighlightbg", STRING, &selhighlightbg},
    {"normhighlightfg", STRING, &normhighlightfg},
    {"normhighlightbg", STRING, &normhighlightbg},
    {"bordercolor", STRING, &bordercolor},
    {"borderwidth", INTEGER, &border_width},
    {"prompt", STRING, &prompt},
    {"fuzzy", INTEGER, &fuzzy},
};

/* ── Word Deletion Characters ──────────────────────────── */
static const char worddelimiters[] = " ";
