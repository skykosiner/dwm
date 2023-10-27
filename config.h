/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const char *fonts[] = {"monospace:size=13"};
static const char dmenufont[] = "monospace:size=12";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[] = "#005577";
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = {col_gray3, col_gray1, col_gray2},
	[SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{"Gimp", NULL, NULL, 0, 1, -1},
	{"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[]=", tile}, /* first entry is default */
	{"><>", NULL}, /* no layout function means floating behavior */
	{"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod1Mask
#define SUPER Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-h", "30"};
static const char *termcmd[] = {"st", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},

    // Rotate stack
    {MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},

    // Open the ~/temp.md file
    {SUPER, XK_t, spawn, SHCMD("vim ~/temp.md")},

    // Pass menu
    {MODKEY | ShiftMask, XK_p, spawn, SHCMD("~/.local/bin/general-scripts/passmenu")},

	// Edit configs in $EDITOR
	{MODKEY, XK_e, spawn, SHCMD("~/.local/bin/general-scripts/configEdit")},

	// Open browser
	{MODKEY | ShiftMask, XK_w, spawn, SHCMD("/usr/bin/brave")},

	// Set background
	{SUPER, XK_w, spawn, SHCMD("sxiv -t $(find ~/.dotfiles/anime | sort)")},

	// Screen brightness
	{SUPER, XK_bracketleft, spawn,
	 SHCMD("~/.dotfiles/bin/.local/bin/general-scripts/brightness up")},
	{SUPER, XK_braceleft, spawn,
	 SHCMD("~/.dotfiles/bin/.local/bin/general-scripts/brightness down")},

	// Aircon control
	{SUPER | ShiftMask, XK_o, spawn,
	 SHCMD("~/.local/bin/aircon-stuff/aircon toggle")},
	{SUPER | ShiftMask, XK_c, spawn, SHCMD("~/.local/bin/aircon-stuff/cold")},
	{SUPER | ShiftMask, XK_h, spawn, SHCMD("~/.local/bin/aircon-stuff/hot")},
	{MODKEY | SUPER, XK_c, spawn,
	 SHCMD("~/.local/bin/aircon-stuff/aircon conflict")},
	{SUPER, XK_n, spawn,
	 SHCMD("~/.local/bin/aircon-stuff/airconControl -change")},
	{SUPER, XK_h, spawn, SHCMD("~/.local/bin/aircon-stuff/aircon hot")},
	{SUPER, XK_c, spawn, SHCMD("~/.local/bin/aircon-stuff/aircon cold")},
	{SUPER, XK_f, spawn,
	 SHCMD("~/.local/bin/aircon-stuff/airconControl -sellect-fan")},

	// Light control
	{SUPER | ShiftMask, XK_l, spawn, SHCMD("~/.local/bin/misc/lights ceiling")},
	{SUPER | ShiftMask, XK_r, spawn, SHCMD("~/.local/bin/misc/lights others")},
	{SUPER | ShiftMask, XK_a, spawn, SHCMD("~/.local/bin/misc/lights all")},
	{SUPER | ShiftMask, XK_m, spawn,
	 SHCMD("~/.local/bin/window-manger/mointers")},

	{SUPER | ShiftMask, XK_g, spawn,
	 SHCMD("~/.local/bin/general-scripts/picomToggle")},

	// Spotify
	{SUPER, XK_p, spawn,
	 SHCMD("~/.local/bin/window-manger/spotify-control -togglePlay")},
	{SUPER, XK_period, spawn,
	 SHCMD("~/.local/bin/window-manger/spotify-control -next")},
	{SUPER, XK_comma, spawn,
	 SHCMD("~/.local/bin/window-manger/spotify-control -prev")},
	{SUPER | ShiftMask, XK_p, spawn, SHCMD("spotify-launcher")},
	{MODKEY, XK_a, spawn,
	 SHCMD("~/.local/bin/window-manger/spotify-control -select-album")},
	{MODKEY | ShiftMask, XK_s, spawn,
	 SHCMD("~/.local/bin/window-manger/spotify-control -select-playlist")},
	// Lyrics
	{SUPER, XK_l, spawn, SHCMD("st -e sptlrx")},

	// Pulse Audio controls
	{SUPER, XK_plus, spawn,
	 SHCMD("~/.local/bin/general-scripts/volumeControl up")},
	{SUPER, XK_minus, spawn,
	 SHCMD("~/.local/bin/general-scripts/volumeControl down")},
	{SUPER, XK_m, spawn,
	 SHCMD("~/.local/bin/general-scripts/volumeControl mute")},

	// File stuff
	{SUPER, XK_o, spawn, SHCMD("st -e ~/.local/bin/lfub")},

	// Screenshot
	{SUPER | ShiftMask, XK_s, spawn,
	 SHCMD("flameshot gui -p ~/Pictures/Captures")},

	// Change background
	{MODKEY | ShiftMask, XK_b, spawn,
	 SHCMD("~/.local/bin/general-scripts/change_background_dmenu")},
	{MODKEY | ShiftMask, XK_t, spawn,
	 SHCMD("~/.local/bin/general-scripts/change_background_test_dmenu")},
	{SUPER, XK_t, spawn,
	 SHCMD("~/.local/bin/general-scripts/change_background_random_test")},

	// Keyboard stuff
	{SUPER, XK_d, spawn, SHCMD("setxkbmap -layout real-prog-dvorak")},
	{SUPER, XK_q, spawn, SHCMD("setxkbmap -layout us")},

	// Emoji
	{SUPER | ShiftMask, XK_e, spawn,
	 SHCMD("~/.local/bin/general-scripts/menu-emoji")},

	// Snipets
	{SUPER, XK_s, spawn, SHCMD("~/.local/bin/snipets")},

    // Captilaz sentences
    {MODKEY, XK_c, spawn, SHCMD("~/.local/bin/capitalizeSentence")},

	// FIND ME DADDY
    {MODKEY | ShiftMask, XK_c, spawn, SHCMD("~/.local/bin/misc/find-me-daddy -add")},
	{SUPER | ShiftMask, XK_f, spawn, SHCMD("~/.local/bin/misc/find-me-daddy -get-fuzzy")},

	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},
	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY, XK_d, incnmaster, {.i = -1}},
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},
	{MODKEY | ShiftMask, XK_Return, zoom, {0}},
	{MODKEY, XK_Tab, view, {0}},
	{MODKEY | ShiftMask, XK_q, killclient, {0}},
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
	{MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
	{MODKEY, XK_space, setlayout, {0}},
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},
	{MODKEY, XK_0, view, {.ui = ~0}},
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
	TAGKEYS(XK_plus, 0)
	TAGKEYS(XK_bracketleft, 1)
	TAGKEYS(XK_braceleft, 2)
	TAGKEYS(XK_parenleft, 3)
	TAGKEYS(XK_ampersand, 4)
	TAGKEYS(XK_equal, 5)
	TAGKEYS(XK_parenright, 6)
	TAGKEYS(XK_braceright, 7)
	TAGKEYS(XK_bracketright, 8)
	{MODKEY | ShiftMask, XK_r, quit, {0}},

	// Gaps
	{SUPER | ShiftMask, XK_minus, setgaps, {.i = -10}},
	{SUPER | ShiftMask, XK_equal, setgaps, {.i = +10}},
	{SUPER, XK_equal, setgaps, {.i = 0}},

	// Fullscreen
	{MODKEY | ShiftMask, XK_f, togglefullscr, {0}},

    // Today list in todoist
    {MODKEY | ShiftMask, XK_space, spawn, SHCMD("/home/sky/.local/bin/todoist --today")},
    // Quick add a task to todoist
    {SUPER, XK_space, spawn, SHCMD("/home/sky/.local/bin/quick-add-task")},
    // Quick add note to obisdian
    {SUPER | ShiftMask, XK_space, spawn, SHCMD("/home/sky/.local/bin/obsidian-cli quick-note")},

    // Clipboard manger
    {SUPER, XK_backslash, spawn, SHCMD("clipmenu")},

    // Mount usb
    {SUPER, XK_u, spawn, SHCMD("/home/sky/.local/bin/mount-usb")},

    // Email with neomutt
	{SUPER, XK_e, spawn, SHCMD("st -e neomutt")},
};

/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, MODKEY, Button1, tag, {0}},
	{ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
