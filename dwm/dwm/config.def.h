/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx        = 1;        /* border pixel of windows */
static unsigned int snap            = 32;       /* snap pixel */
static int showbar                  = 1;        /* 0 means no bar */
static int topbar                   = 1;        /* 0 means bottom bar */
static char font[]                  = "JetBrainsMono Nerd Font:size=11";
static char dmenufont[]             = "JetBrainsMono Nerd Font:size=12";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* status padding */
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 2;        /* vertical padding for statusbar */

typedef struct {
   const char *name;
   const void *cmd;
} Sp;
const char *spcmd1[] = {"alacritty", "--class", "spterm", "--option", "window.dimensions.columns=120", "--option", "window.dimensions.lines=34", NULL};
const char *spcmd2[] = {"alacritty", "--class", "spfm", "--option", "window.dimensions.columns=144", "--option", "window.dimensions.lines=41", "-e", "yazi", NULL};
static Sp scratchpads[] = {
   /* name       cmd  */
   {"spterm",    spcmd1},
   {"spyazi",    spcmd2},
};

/* gaps (for vanitygaps patch) */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 8;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* class             instance    title       tags mask     isfloating   monitor */
  { "zen",             NULL,       NULL,       1 << 1,       0,           -1 },
	{ "firefox",         NULL,       NULL,       1 << 1,       0,           -1 },
  { "Brave-browser",   NULL,       NULL,       1 << 1,       0,           -1 },
  { "Code",            NULL,       NULL,       1 << 2,       0,           -1 },
  { "discord",         NULL,       NULL,       1 << 3,       0,           -1 },
  { "Gimp",            NULL,       NULL,       1 << 5,       0,           -1 },
  { "Inkscape",        NULL,       NULL,       1 << 5,       0,           -1 },
  { "kdenlive",        NULL,       NULL,       1 << 6,       0,           -1 },
	{ "obs",             NULL,       NULL,       1 << 8,       0,           -1 },
  { NULL,              "spterm",   NULL,       SPTAG(0),     1,           -1 },
  { NULL,              "spfm",     NULL,       SPTAG(1),     1,           -1 },
  { NULL,              "keepassxc",NULL,       SPTAG(2),     0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "tatami.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
    { "|+|",      tatami },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG)                                                                                               \
       &((Keychord){1, {{MODKEY, KEY}},                                        view,           {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask, KEY}},                            toggleview,     {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ShiftMask, KEY}},                              tag,            {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, KEY}},                  toggletag,      {.ui = 1 << TAG} }),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#include <X11/XF86keysym.h>

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[] = { "alacritty", NULL };
static const char *fmthunar[] = { "thunar", NULL };

/* Volome Control */
static const char *upvol[]   = { "pamixer", "--increase", "5", NULL };
static const char *downvol[] = { "pamixer", "--decrease", "5", NULL };
static const char *mutevol[] = { "pamixer", "--toggle-mute", NULL };
static const char *unmutevol[] = { "pamixer", "--unmute", NULL };

/* Screenshot commands (POSIX sh compatible) */
static const char *scrotfullclip[] = { "sh", "-c", "~/.config/scripts/screenshots/scrotfullclip.sh", NULL };
static const char *scrotselclip[]  = { "sh", "-c", "~/.config/scripts/screenshots/scrotselclip.sh", NULL };
static const char *scrotwinclip[]  = { "sh", "-c", "~/.config/scripts/screenshots/scrotwinclip.sh", NULL };
// static const char *scrotfullfile[] = { "sh", "-c", "~/.config/scripts/screenshots/scrotfullfile.sh", NULL };
// static const char *scrotselfile[]  = { "sh", "-c", "~/.config/scripts/screenshots/scrotselfile.sh", NULL };

// Scripts
static const char *bookmarks[] = { "sh", "-c", "~/.config/scripts/bookmarks/bookmarks-dmenu.sh", NULL };
static const char *wallpapers[] = { "sh", "-c", "~/.config/scripts/wallpapers/wallpapers-dmenu.sh", NULL };
static const char *session[] = { "sh", "-c", "~/.config/scripts/system-menu/sm-dmenu.sh", NULL };
static const char *repos[] = { "sh", "-c", "~/.config/scripts/project-repos/pr-dmenu.sh", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
               { "font",               STRING,  &font },
               { "dmenufont",          STRING,  &dmenufont },
               { "normbgcolor",        STRING,  &normbgcolor },
               { "normbordercolor",    STRING,  &normbordercolor },
               { "normfgcolor",        STRING,  &normfgcolor },
               { "selbgcolor",         STRING,  &selbgcolor },
               { "selbordercolor",     STRING,  &selbordercolor },
               { "selfgcolor",         STRING,  &selfgcolor },
               { "borderpx",           INTEGER, &borderpx },
               { "snap",               INTEGER, &snap },
               { "showbar",            INTEGER, &showbar },
               { "topbar",             INTEGER, &topbar },
               { "nmaster",            INTEGER, &nmaster },
               { "resizehints",        INTEGER, &resizehints },
               { "mfact",              FLOAT,   &mfact },
};

#include "movestack.c"
static Keychord *keychords[] = { /* Program launcher & system */
    &((Keychord){1, {{MODKEY, XK_d}},                          spawn,          {.v = dmenucmd } }),
    &((Keychord){1, {{MODKEY, XK_e}},                          spawn,          {.v = fmthunar } }),
    &((Keychord){1, {{MODKEY, XK_Return}},                     spawn,          {.v = termcmd } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_b}},                togglebar,      {0} }),
    &((Keychord){1, {{MODKEY, XK_j}},                          focusstack,     {.i = +1 } }),
    &((Keychord){1, {{MODKEY, XK_k}},                          focusstack,     {.i = -1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_j}},                movestack,      {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_k}},                movestack,      {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_c}},                          incnmaster,     {.i = +1 } }),
    &((Keychord){1, {{MODKEY, XK_p}},                          incnmaster,     {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_h}},                          setmfact,       {.f = -0.05} }),
    &((Keychord){1, {{MODKEY, XK_l}},                          setmfact,       {.f = +0.05} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_h}},                setcfact,       {.f = +0.25} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_l}},                setcfact,       {.f = -0.25} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_o}},                setcfact,       {.f =  0.00} }),
    &((Keychord){1, {{MODKEY, XK_z}},                          zoom,           {0} }),
    &((Keychord){1, {{MODKEY, XK_Tab}},                        view,           {0} }),
    &((Keychord){1, {{MODKEY, XK_q}},                          killclient,     {0} }),
    &((Keychord){1, {{MODKEY, XK_space}},                      setlayout,      {0} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_space}},            togglefloating, {0} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_f}},                togglefullscr,  {0} }),
    &((Keychord){1, {{MODKEY, XK_0}},                          view,           {.ui = ~0 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_0}},                tag,            {.ui = ~0 } }),
    &((Keychord){1, {{MODKEY, XK_comma}},                      focusmon,       {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_period}},                     focusmon,       {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_comma}},            tagmon,         {.i = -1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_period}},           tagmon,         {.i = +1 } }),
    
    /* Monitor toggletag */
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_t}, {0, XK_1}},   toggletag,      {.ui = 1 << 0} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_t}, {0, XK_2}},   toggletag,      {.ui = 1 << 1} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_t}, {0, XK_3}},   toggletag,      {.ui = 1 << 2} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_t}, {0, XK_4}},   toggletag,      {.ui = 1 << 3} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_t}, {0, XK_5}},   toggletag,      {.ui = 1 << 4} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_t}, {0, XK_6}},   toggletag,      {.ui = 1 << 5} }),

    /* Monitor tagnextmon */
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_n}, {0, XK_1}},   tagnextmon,     {.ui = 1 << 0} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_n}, {0, XK_2}},   tagnextmon,     {.ui = 1 << 1} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_n}, {0, XK_3}},   tagnextmon,     {.ui = 1 << 2} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_n}, {0, XK_4}},   tagnextmon,     {.ui = 1 << 3} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_n}, {0, XK_5}},   tagnextmon,     {.ui = 1 << 4} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_n}, {0, XK_6}},   tagnextmon,     {.ui = 1 << 5} }),

    /* Monitor tagprevmon */
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_p}, {0, XK_1}},   tagprevmon,     {.ui = 1 << 0} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_p}, {0, XK_2}},   tagprevmon,     {.ui = 1 << 1} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_p}, {0, XK_3}},   tagprevmon,     {.ui = 1 << 2} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_p}, {0, XK_4}},   tagprevmon,     {.ui = 1 << 3} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_p}, {0, XK_5}},   tagprevmon,     {.ui = 1 << 4} }),
    &((Keychord){3, {{MODKEY, XK_t}, {0, XK_p}, {0, XK_6}},   tagprevmon,     {.ui = 1 << 5} }),

    /* Volume control */
    &((Keychord){1, {{0, XF86XK_AudioRaiseVolume}},            spawn,          {.v = upvol } }),
    &((Keychord){1, {{0, XF86XK_AudioLowerVolume}},            spawn,          {.v = downvol } }),
    &((Keychord){1, {{0, XF86XK_AudioMute}},                   spawn,          {.v = mutevol } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_m}},                spawn,          {.v = unmutevol} }),

    /* Scripts */
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_b}},               spawn,          {.v = bookmarks} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_w}},               spawn,          {.v = wallpapers} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_s}},               spawn,          {.v = session} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_p}},               spawn,          {.v = repos} }),

    /* Scratchpads */
    &((Keychord){2, {{MODKEY, XK_s}, {0, XK_t}},               togglescratch,  {.ui = 0} }),
    &((Keychord){2, {{MODKEY, XK_s}, {0, XK_y}},               togglescratch,  {.ui = 1} }),

    /* Screen lock */
    &((Keychord){1, {{MODKEY|ShiftMask, XK_x}},                spawn,          SHCMD("betterlockscreen -l dimblur") }),

    /* Screenshot */
    &((Keychord){1, {{0, XK_Print}},                           spawn,          {.v = scrotselclip} }),
    &((Keychord){1, {{MODKEY, XK_Print}},                      spawn,          {.v = scrotfullclip} }),
    &((Keychord){1, {{ControlMask, XK_Print}},                 spawn,          {.v = scrotwinclip} }),
    // &((Keychord){2, {{MODKEY, XK_s}, {0, XK_Print}},           spawn,          {.v = scrotfullfile} }),
    // &((Keychord){2, {{MODKEY, XK_s}, {0, XK_s}},               spawn,          {.v = scrotselfile} }),

    // Quit dwm
    &((Keychord){1, {{MODKEY|ShiftMask, XK_q}},                quit,           {0} }),
    &((Keychord){1, {{MODKEY, XK_r}},                          quit,           {1} }),

    /* Vanitygaps */
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_u}},               incrgaps,       {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_u}},       incrgaps,       {.i = -1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_i}},               incrigaps,      {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_i}},       incrigaps,      {.i = -1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_o}},               incrogaps,      {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_o}},       incrogaps,      {.i = -1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_6}},               incrihgaps,     {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_6}},       incrihgaps,     {.i = -1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_7}},               incrivgaps,     {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_7}},       incrivgaps,     {.i = -1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_8}},               incrohgaps,     {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_8}},       incrohgaps,     {.i = -1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_9}},               incrovgaps,     {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_9}},       incrovgaps,     {.i = -1 } }),
    &((Keychord){2, {{MODKEY, XK_i}, {0, XK_0}},               togglegaps,     {0} }),
    &((Keychord){2, {{MODKEY, XK_i}, {ShiftMask, XK_0}},       defaultgaps,    {0} }),

    /* Layouts */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_t}},               setlayout,      {.v = &layouts[0]} }),  /* []= tile */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_m}},               setlayout,      {.v = &layouts[1]} }),  /* [M] monocle */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_s}},               setlayout,      {.v = &layouts[2]} }),  /* [@] spiral */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_d}},               setlayout,      {.v = &layouts[3]} }),  /* [\] dwindle */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_h}},               setlayout,      {.v = &layouts[4]} }),  /* H[] deck */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_b}},               setlayout,      {.v = &layouts[5]} }),  /* TTT bstack */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_v}},               setlayout,      {.v = &layouts[6]} }),  /* === bstackhoriz */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_g}},               setlayout,      {.v = &layouts[7]} }),  /* HHH grid */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_n}},               setlayout,      {.v = &layouts[8]} }),  /* ### nrowgrid */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_r}},               setlayout,      {.v = &layouts[9]} }),  /* --- horizgrid */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_c}},               setlayout,      {.v = &layouts[10]} }), /* ::: gaplessgrid */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_y}},               setlayout,      {.v = &layouts[11]} }), /* |M| centeredmaster */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_f}},               setlayout,      {.v = &layouts[12]} }), /* >M> centeredfloatingmaster */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_a}},               setlayout,      {.v = &layouts[13]} }), /* |+| tatami */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_i}},               setlayout,      {.v = &layouts[14]} }), /* <>> floating */
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_space}},           setlayout,      {0} }),                 /* toggle last/floating */

    /* Tag keys */
    TAGKEYS( XK_1, 0) TAGKEYS( XK_2, 1) TAGKEYS( XK_3, 2)
    TAGKEYS( XK_4, 3) TAGKEYS( XK_5, 4) TAGKEYS( XK_6, 5)
    TAGKEYS( XK_7, 6) TAGKEYS( XK_8, 7) TAGKEYS( XK_9, 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

