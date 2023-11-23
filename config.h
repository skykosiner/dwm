/* See LICENSE file for copyright and license details. */

#define TERMCLASS "St"

/* appearance */
#include <X11/X.h>
static const unsigned int borderpx = 3; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int gappx     = 30;        /* gaps between windows */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const char *font[] = {"FiraCode Nerd Font Mono:size=15"};
static const char dmenufont[] = "monospace:size=12";
static const char *fonts[] = { font };
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
    /* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
    // { "Gimp",     NULL,       NULL,       	    1 << 5,       0,           0,         0,        -1 },
    { TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
    { NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"🌈", tile}, /* first entry is default */
	{"💤", NULL}, /* no layout function means floating behavior */
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
    // Tag keys (have weird looking keys for the tag due to my keybord laytou)
	TAGKEYS(XK_plus, 0)
	TAGKEYS(XK_bracketleft, 1)
	TAGKEYS(XK_braceleft, 2)
	TAGKEYS(XK_parenleft, 3)
	TAGKEYS(XK_ampersand, 4)
	TAGKEYS(XK_equal, 5)
	TAGKEYS(XK_parenright, 6)
	TAGKEYS(XK_braceright, 7)
	TAGKEYS(XK_bracketright, 8)

    // Moving around windows and stuff
    // Rotate stack
    {MODKEY | ShiftMask, XK_j, rotatestack, {.i = -1}},
    {MODKEY | ShiftMask, XK_k, rotatestack, {.i = +1}},

    // Toggle bar
	{MODKEY, XK_b, togglebar, {0}},

    // Move around the stac
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},

    // Increase or decrase the number of masters
	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY, XK_d, incnmaster, {.i = -1}},

    // Resize master
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},

    // Make window the master
	{MODKEY | ShiftMask, XK_Return, zoom, {0}},

    // Go back to last tag
	{MODKEY, XK_Tab, view, {0}},

    // Kill current selected window
	{MODKEY | ShiftMask, XK_q, killclient, {0}},

    // Change between floating and tiling layout (tiling: 🌈 floating: 💤)
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
	// {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},

    // Change between all set layouts but as I only have two it will just change between floating and tiling
	{MODKEY, XK_space, setlayout, {0}},

    // Toggle floating for currently selected window
	{SUPER | ShiftMask, XK_f, togglefloating, {0}},

    // View every tag on one screen at once
	{MODKEY, XK_asterisk, view, {.ui = ~0}},

    // Make a window stick to every tag
	{MODKEY | ShiftMask, XK_asterisk, tag, {.ui = ~0}},

    // Switch focus between screens
	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},

    // Move window between screens
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = -1}},

    // Quit dwm
	// {MODKEY | ShiftMask, XK_r, quit, {0}},

	// Gaps
	{SUPER | ShiftMask, XK_minus, setgaps, {.i = -10}},
	{SUPER | ShiftMask, XK_equal, setgaps, {.i = +10}},
	{SUPER, XK_equal, setgaps, {.i = 0}},

	// Fullscreen
	{MODKEY | ShiftMask, XK_f, togglefullscr, {0}},

    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},

    // Time tracking funn
    {SUPER, XK_t, spawn, SHCMD("~/.local/bin/time-tracking")},

    // Pass menu
    {MODKEY | ShiftMask, XK_p, spawn, SHCMD("~/.local/bin/passmenu")},

	// Edit configs in $EDITOR
	{MODKEY, XK_e, spawn, SHCMD("~/.local/bin/configEdit")},

	// Open browser
	{MODKEY | ShiftMask, XK_w, spawn, SHCMD("/usr/bin/brave")},

	// Set background
	{SUPER, XK_w, spawn, SHCMD("sxiv -t $(find ~/.dotfiles/backgrounds/ | sort)")},

	// Screen brightness
	{SUPER, XK_bracketleft, spawn,
	 SHCMD("~/.dotfiles/bin/.local/bin/brightness up")},
	{SUPER, XK_braceleft, spawn,
	 SHCMD("~/.dotfiles/bin/.local/bin/brightness down")},

	// Aircon control
	{SUPER | ShiftMask, XK_o, spawn,
	 SHCMD("~/.local/bin/aircon toggle")},
	{SUPER | ShiftMask, XK_c, spawn, SHCMD("~/.local/bin/cold")},
	{SUPER | ShiftMask, XK_h, spawn, SHCMD("~/.local/bin/hot")},
	{MODKEY | SUPER, XK_c, spawn,
	 SHCMD("~/.local/bin/aircon conflict")},
	{SUPER, XK_n, spawn,
	 SHCMD("~/.local/bin/airconControl -change")},
	{SUPER, XK_h, spawn, SHCMD("~/.local/bin/aircon hot")},
	{SUPER, XK_c, spawn, SHCMD("~/.local/bin/aircon cold")},
	{SUPER, XK_f, spawn,
	 SHCMD("~/.local/bin/airconControl -sellect-fan")},

	// Light control
	{SUPER | ShiftMask, XK_l, spawn, SHCMD("~/.local/bin/lights ceiling")},
	{SUPER | ShiftMask, XK_r, spawn, SHCMD("~/.local/bin/lights others")},
	{SUPER | ShiftMask, XK_a, spawn, SHCMD("~/.local/bin/lights all")},
    {MODKEY | ShiftMask, XK_l, spawn, SHCMD("~/.local/bin/light-percent")},
    {MODKEY | ShiftMask, XK_l, spawn, SHCMD("~/.local/bin/light-percent")},
    {MODKEY | ShiftMask, XK_c, spawn, SHCMD("~/.local/bin/light-temp")},


	{SUPER | ShiftMask, XK_m, spawn,
	 SHCMD("~/.local/bin/screens")},

	{SUPER | ShiftMask, XK_g, spawn,
	 SHCMD("~/.local/bin/picomToggle")},

	// Spotify
	{SUPER, XK_p, spawn,
	 SHCMD("~/.local/bin/spotify-control -togglePlay")},
	{SUPER, XK_period, spawn,
	 SHCMD("~/.local/bin/spotify-control -next")},
	{SUPER, XK_comma, spawn,
	 SHCMD("~/.local/bin/spotify-control -prev")},
	{SUPER | ShiftMask, XK_p, spawn, SHCMD("~/.local/bin/spotify-control -play-song")},
	{MODKEY, XK_a, spawn,
	 SHCMD("~/.local/bin/spotify-control -select-album")},
	{MODKEY | ShiftMask, XK_s, spawn,
	 SHCMD("~/.local/bin/spotify-control -select-playlist")},
	{MODKEY | ShiftMask, XK_m, spawn, SHCMD("st -e spt")},

	// Lyrics
	{SUPER, XK_l, spawn, SHCMD("st -e sptlrx")},

	// Pulse Audio controls
	{SUPER, XK_plus, spawn,
	 SHCMD("~/.local/bin/volumeControl up")},
	{SUPER, XK_minus, spawn,
	 SHCMD("~/.local/bin/volumeControl down")},
	{SUPER, XK_m, spawn,
	 SHCMD("~/.local/bin/volumeControl mute")},

	// File stuff
	{SUPER, XK_o, spawn, SHCMD("st -e ~/.local/bin/lfub")},

	// Screenshot
	{SUPER | ShiftMask, XK_s, spawn,
	 SHCMD("flameshot gui -p ~/Pictures/Captures")},

	// Change background
	{MODKEY | ShiftMask, XK_b, spawn,
	 SHCMD("~/.local/bin/change_background_dmenu")},

	{SUPER | ShiftMask, XK_t, spawn,
	 SHCMD("~/.local/bin/torrents -dmenu")},

	// Keyboard stuff
	{SUPER, XK_d, spawn, SHCMD("setxkbmap -layout real-prog-dvorak")},
	{SUPER, XK_q, spawn, SHCMD("setxkbmap -layout us")},

	// Emoji
	{SUPER | ShiftMask, XK_e, spawn,
	 SHCMD("~/.local/bin/menu-emoji")},

	// Snipets
	{SUPER, XK_s, spawn, SHCMD("~/.local/bin/snipets")},

    // Captilaz sentences
    // {MODKEY, XK_c, spawn, SHCMD("~/.local/bin/capitalizeSentence")},


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

    // System stuff
    {MODKEY, XK_s, spawn, SHCMD("/home/sky/.local/bin/system-action")},

    // Run remaps script
    {MODKEY, XK_F1, spawn, SHCMD("/home/sky/.local/bin/keyboard-connected")},

    // Open pulsemixer
    {MODKEY, XK_F2, spawn, SHCMD("st -e pulsemixer")},

    // Open thunar
    {MODKEY, XK_F3, spawn, SHCMD("thunar")},
};

/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function argument */
	{ ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ ClkWinTitle, 0, Button2, zoom, {0}},
	// {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ ClkTagBar, 0, Button1, view, {0}},
	{ ClkTagBar, 0, Button3, toggleview, {0}},
	{ ClkTagBar, MODKEY, Button1, tag, {0}},
	{ ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
