/* See LICENSE file for copyright and license details. */

/* appearance */
static const int vertpad     = 5;       /* vertical padding of bar */
static const int sidepad     = 5;       /* horizontal padding of bar */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static  int topbar                  = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Maple Mono NF CN:size=10", "D2Coding Nerd Font:size=10"}; 
static char dmenufont[]       = "Maple Mono NF CN:size=10";


static char normbgcolor[]       = "#0a0a0a";
static char normbordercolor[]   = "#1a1a1a";
static char normfgcolor[]       = "#6a6a6a";
static char selfgcolor[]        = "#a8a8a8";
static char selbordercolor[]    = "#C2185B"; /* Heavily desaturated, earthy red */
static char selbgcolor[]        = "#C2185B"; /* Dark, near-charcoal red */

static char *colors[][3]      = {
    /*                  fg         bg         border   */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
        /* for bar --> {text, background, null} */
    [SchemeStatus]  = { normfgcolor, normbgcolor,  normbgcolor  }, /* status R */
    [SchemeTagsSel]  = { selfgcolor,  selbgcolor,  selbordercolor }, /* tag L selected (Muted) */
    [SchemeTagsNorm]  = { normfgcolor, normbgcolor,  normbgcolor  }, /* tag L unselected (Grey) */
    [SchemeInfoSel]  = { normbgcolor,  selbgcolor,  normbgcolor  }, /* info M selected (Muted bg) */
    [SchemeInfoNorm]  = { normfgcolor, normbgcolor,  normbgcolor  }, /* info M unselected */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "st",       NULL,       NULL,       0,            0,           -1 },
	{ "librewolf",NULL,       NULL,       1 << 1,       0,           -1 },
	{ "steam",    NULL,       NULL,       1 << 2,       0,           -1 }, 
    { "discord",  NULL,       NULL,       1 << 4,       0,           -1 },
    { "keepassxc",NULL,       NULL,       0,            1,           -1 },
    { "st-notes", NULL,       NULL,       0,            1,           -1 },
    { "Anki",     NULL,       NULL,       0,            1,           -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

#define FORCE_VSPLIT 1
#include "vanitygaps.c"

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
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"
#define BROWSER "librewolf-bin"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *prtscrcmd[] = { "flameshot", "gui", "-p", "/home/aerna/Documents/screenshot", NULL};
static const char *freetubecmd[] = { "flatpak", "run", "io.freetubeapp.FreeTube", NULL };
static const char *ankicmd[]  = { "flatpak", "run", "net.ankiweb.Anki", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_z,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,	                    XK_F5,     xrdb,	       {.v = NULL } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_k,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_BackSpace,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      quit,           {1} },

	/* gaps control */
	{ MODKEY,			XK_minus,  incrgaps,       {.i = -3 } }, /* all */
	{ MODKEY,			XK_equal,  incrgaps,       {.i = +3 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } }, /* inner */
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } }, /* outer */
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } }, /* inner horiz */
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } }, /* inner vert */
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } }, /* outer horiz */
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } }, /* outer vert */
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_equal,  togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_minus,  defaultgaps,    {0} },

	/* app binds */
	{ MODKEY,			    XK_m,      spawn,      {.v = (const char*[]){ "st", "-e", "rmpc", NULL } } },
	{ MODKEY,		     	XK_w,      spawn,      {.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY,		     	XK_d,      spawn,      {.v = (const char*[]){ "discord", NULL } } },
	{ MODKEY,		     	XK_s,      spawn,      {.v = (const char*[]){ "steam", NULL } } },
	{ MODKEY,               XK_a,      spawn,      {.v = ankicmd } },
	{ MODKEY,               XK_y,      spawn,      {.v = freetubecmd } },
    { 0,                    XK_Print,  spawn,      {.v = prtscrcmd } },

  /* other bindings */
	{ MODKEY,				XK_F12,    spawn,       SHCMD("playerctl -p mpd next") },
	{ MODKEY,				XK_F11,    spawn,       SHCMD("playerctl -p mpd play-pause") },
	{ MODKEY,				XK_F10,    spawn,       SHCMD("playerctl -p mpv play-pause") },
        { MODKEY,				XK_F8,     spawn,       SHCMD("slock") },
        { MODKEY,				XK_v,     spawn,       SHCMD("pavucontrol") },
	{ MODKEY|ShiftMask,		        XK_n,      spawn,       {.v = (const char*[]){ "notes", NULL } } },
	{ MODKEY|ShiftMask,		        XK_d,      spawn,       {.v = (const char*[]){ "sys", NULL } } },
	{ MODKEY|ShiftMask,		        XK_h,      spawn,       {.v = (const char*[]){ "clipboard", NULL } } },
	{ MODKEY|ShiftMask,		        XK_b,      spawn,       {.v = (const char*[]){ "bookmarks", NULL } } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

