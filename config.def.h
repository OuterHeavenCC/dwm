/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int default_border = 0;  // to switch back to default border after dynamic border resizing via keybinds
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails,display systray on the 1st monitor,False: display systray on last monitor*/
static const int showsystray        = 0;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always };
static const int showtab            = showtab_auto;
static const int toptab             = True;
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 5;
static const int vertpadbar         = 11;
static const int vertpadtab         = 33;
static const int horizpadtabi       = 15;
static const int horizpadtabo       = 15;
static const int scalepreview       = 4;
static       int tag_preview        = 0;        /* 1 means enable, 0 is off */

static const char *fonts[]          = { "FiraCode Nerd Font:style:medium:size=10",
                                        "Material Design Icons-Regular:size=10",
                                      };
static const char dmenufont[]       = "FiraCode Nerd Font:size=10";
static const int colorfultag        = 1;  /* 0 means use SchemeSel for selected non vacant tag */

// theme
#include "themes/nord.h"

static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm]       = { gray3, black, gray2 },
    [SchemeSel]        = { gray4, blue,  blue  },
    [TabSel]           = { blue, gray2,  black  },
    [TabNorm]          = { gray3, black, black },
    [SchemeTag]        = { gray3, black, black },
    [SchemeTag1]       = { blue,  black, black },
    [SchemeTag2]       = { red,   black, black },
    [SchemeTag3]       = { orange, black,black },
    [SchemeTag4]       = { green, black, black },
    [SchemeTag5]       = { pink,  black, black },
    [SchemeTag6]       = { yellow,  black, black },
    [SchemeTag7]       = { blue,  black, black },
    [SchemeTag8]       = { red,  black, black },
    [SchemeTag9]       = { orange,  black, black },
    [SchemeLayout]     = { green, black, black }, 
    [SchemeBtnPrev]    = { green, black, black }, 
    [SchemeBtnNext]    = { yellow, black, black }, 
    [SchemeBtnClose]   = { red, black, black }, 
};

/* tagging */
static char *tags[] = {"??? ", "??? ", "??? ", "??? ", "??? ", "??? ", "??? ", "??? ", "??? "};

static const int tagschemes[] = { SchemeTag1, SchemeTag2, SchemeTag3,
                                  SchemeTag4, SchemeTag5, SchemeTag6,
                                  SchemeTag7, SchemeTag8, SchemeTag9,
                                };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
  /* class     instance  title           tags mask  iscentered  isfloating   isterminal noswallow  monitor */
  { "Gimp",    NULL,     NULL,           0,         1,          1,           0,         0,         -1 },
  { "Firefox", NULL,     NULL,           1 << 8,    0,          0,           0,        -1,         -1 },
  { TERMCLASS, NULL,     NULL,           0,         0,          0,           1,         0,         -1 },
  { NULL,      NULL,     "Event Tester", 0,         0,          0,           0,         1,         -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "functions.h"


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

/* commands */
static char dmenumon[2] = "0"; /* component of jmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = {  "st", NULL }; // change this to your term

#include <X11/XF86keysym.h>
static Key keys[] = {
    /* Window manager shortcuts */
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd }},  

    { MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
    { MODKEY|ControlMask,           XK_w,      tabmode,        { -1 } },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ControlMask,           XK_h,      setcfact,       {.f = +0.25} },
    { MODKEY|ControlMask,           XK_l,      setcfact,       {.f = -0.25} },
    { MODKEY|ControlMask,           XK_o,      setcfact,       {.f =  0.00} },
    { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
    { MODKEY,                       XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },

    // overall gaps
    { MODKEY,                          XK_a,      incrgaps,       {.i = +1 } },
    { MODKEY|ShiftMask,                XK_a,      togglegaps,     {0} },

    { MODKEY,                          XK_z,      incrgaps,       {.i = -1 } },
    { MODKEY|ShiftMask,                XK_z,      defaultgaps,    {0} },

    // inner gaps
    { MODKEY|ShiftMask,                XK_i,      incrigaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },

    // outer gaps
    { MODKEY|ControlMask,              XK_o,      incrogaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },

    { MODKEY|ControlMask,              XK_6,      incrihgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_7,      incrivgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_8,      incrohgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_9,      incrovgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },


    { MODKEY,                       XK_q,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_y,      setlayout,      {.v = &layouts[2]} },
    { MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[10]} },
    { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[13]} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ControlMask,		XK_comma,  cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_Left,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_Right, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_Left,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Right, tagmon,         {.i = +1 } },

    TAGKEYS(                        XK_ampersand,                   0)
    TAGKEYS(                        XK_eacute,                      1)
    TAGKEYS(                        XK_quotedbl,                    2)
    TAGKEYS(                        XK_apostrophe,                  3)
    TAGKEYS(                        XK_parenleft,                   4)
    TAGKEYS(                        XK_minus,                       5)
    TAGKEYS(                        XK_egrave,                      6)
    TAGKEYS(                        XK_underscore,                  7)
    TAGKEYS(                        XK_ccedilla,                    8)
    {MODKEY,                        XK_ccedilla,    view,   {.ui = ~0}},
    {
        MODKEY|ControlMask,         XK_q,      quit,           {0}
    },
    { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
    { MODKEY|ShiftMask,             XK_h,      hidewin,        {0} },
    { MODKEY|ShiftMask,             XK_r,      restorewin,     {0} },

    /* Personal shortcuts */
    { MODKEY,                       XK_BackSpace,   spawn,  SHCMD("sysact")},
    { MODKEY|ShiftMask,             XK_BackSpace,   spawn,  SHCMD("passmenu -c")},

    { MODKEY,                       XK_Delete,      spawn,  SHCMD("dmenukill")},

    { MODKEY,                       XK_b,           spawn,  SHCMD("dmenu-bluetooth")},

    { MODKEY,                       XK_c,           spawn,  SHCMD(TERMINAL " -e calcurse")},
    { MODKEY|ShiftMask,             XK_c,           spawn,  SHCMD(TERMINAL " -e bc -lq")},

    { MODKEY,                       XK_d,           spawn,  SHCMD("dmenu_run -c")},

    { MODKEY,                       XK_e,           spawn,  SHCMD(TERMINAL " -e neomutt")},
    { MODKEY|ShiftMask,             XK_e,           spawn,  SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ""~/.config/abook/addressbook")},

    { MODKEY|ShiftMask,             XK_f,           spawn,  SHCMD(TERMINAL " -e broot")},

    { MODKEY,                       XK_m,           spawn,  SHCMD(TERMINAL " -e ncmpcpp")},
    { MODKEY|ShiftMask,             XK_m,           spawn,  SHCMD("pamixer -t")},

    { MODKEY,                       XK_n,           spawn,  SHCMD(TERMINAL " -e $EDITOR")},
    { MODKEY|ShiftMask,             XK_n,           spawn,  SHCMD(TERMINAL " -e newsboat")},

    { MODKEY,                       XK_p,           spawn,  SHCMD("mpc toggle")},
    { MODKEY|ShiftMask,             XK_p,           spawn,  SHCMD("mpc pause ; pauseallmpv")},

    { MODKEY,                       XK_r,           spawn,  SHCMD(TERMINAL " -e lfimg")},

    { MODKEY,                       XK_s,           spawn,  SHCMD(TERMINAL " -e pulsemixer")},
    { MODKEY|ShiftMask,             XK_s,           spawn,  SHCMD("bmks")},

    { MODKEY,                       XK_v,           spawn,  SHCMD("ytfzf -D")},

    { MODKEY,                       XK_w,           spawn,  SHCMD("$BROWSER")},
    { MODKEY|ShiftMask,             XK_w,           spawn,  SHCMD("networkmanager_dmenu")},

    { MODKEY,                       XK_x,           spawn,  SHCMD("clipmenu")},

    { MODKEY,                       XK_F1,          spawn,  SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -")},
    { MODKEY,                       XK_F2,          spawn,  SHCMD("dmenu_nfi")},
    { MODKEY,                       XK_F3,          spawn,  SHCMD("displayselect")},
    { MODKEY,                       XK_F4,          spawn,  SHCMD(TERMINAL " -e btop")},
    { MODKEY,                       XK_F5,          spawn,  SHCMD("ssh-dmenu")},
    { MODKEY,                       XK_F6,          spawn,  SHCMD("torwrap")},
    { MODKEY,                       XK_F7,          spawn,  SHCMD("td-toggle")},
    { MODKEY,                       XK_F8,          spawn,  SHCMD("mw -Y")},
    { MODKEY,                       XK_F9,          spawn,  SHCMD("dmenumount")},
    { MODKEY,                       XK_F10,         spawn,  SHCMD("dmenuumount")},
    { MODKEY,                       XK_F11,         spawn,  SHCMD("mpv --no-cache --no-osc --no-input-default-bindings ""--input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | ""tail -n 1)")},
    { MODKEY,                       XK_F12,         spawn,  SHCMD("remaps & notify-send \\\"Clavier remapp??\\\"")},

    { 0,                            XK_Print,       spawn,  SHCMD("maimpick")},
    { MODKEY,                       XK_Print,       spawn,  SHCMD("dmenurecord")},
    { MODKEY|ShiftMask,             XK_Print,       spawn,  SHCMD("dmenurecord kill")},

    { MODKEY,                       XK_exclam,      spawn,  SHCMD("telegram-desktop")},

    /* Music mapping (AZERTY Keyboard) */
    { MODKEY,                       XK_comma,       spawn,  SHCMD("mpc seek -5")},
    { MODKEY,                       XK_semicolon,   spawn,  SHCMD("mpc seek +5")},

	  { MODKEY,		                    XK_dead_circumflex,	      spawn,	SHCMD("mpc seek 0%") },
	  { MODKEY|ShiftMask,	            XK_dead_circumflex,	      spawn,	SHCMD("mpc prev") },
	  { MODKEY,         		          XK_dollar,	    spawn,	SHCMD("mpc repeat") },
	  { MODKEY|ShiftMask,		          XK_dollar,	    spawn,	SHCMD("mpc next") },

    { 0,                            XF86XK_AudioMute,                            spawn,   SHCMD("pamixer -t")},
    { 0,                            XF86XK_AudioRaiseVolume,                     spawn,   SHCMD("pamixer --allow-boost -i 3")},
    { 0,                            XF86XK_AudioLowerVolume,                     spawn,   SHCMD("pamixer --allow-boost -d 3")},
    { 0,                            XF86XK_AudioPrev,                            spawn,   SHCMD("mpc prev")},
    { 0,                            XF86XK_AudioNext,                            spawn,   SHCMD("mpc next")},
    { 0,                            XF86XK_AudioPause,                           spawn,   SHCMD("mpc pause")},
    { 0,                            XF86XK_AudioPlay,                            spawn,   SHCMD("mpc play")},
    { 0,                            XF86XK_AudioStop,                            spawn,   SHCMD("mpc stop")},
    { 0,                            XF86XK_AudioMedia,                           spawn,   SHCMD(TERMINAL " -e ncmpcpp")},
    { 0,                            XF86XK_AudioMicMute,                         spawn,   SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle")},
    { 0,                            XF86XK_PowerOff,                             spawn,   SHCMD("sysact")},
    { 0,                            XF86XK_Calculator,                           spawn,   SHCMD(TERMINAL " -e bc -lq")},
    { 0,                            XF86XK_Sleep,                                spawn,   SHCMD("doas -A zzz")},
    { 0,                            XF86XK_WWW,                                  spawn,   SHCMD("$BROWSER")},
    { 0,                            XF86XK_DOS,                                  spawn,   SHCMD(TERMINAL)},
    { 0,                            XF86XK_ScreenSaver,                          spawn,   SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv")},
    { 0,                            XF86XK_TaskPane,                             spawn,   SHCMD(TERMINAL " -e htop")},
    { 0,                            XF86XK_Mail,                                 spawn,   SHCMD(TERMINAL " -e neomutt")},
    { 0,                            XF86XK_MonBrightnessUp,                      spawn,   SHCMD("xbacklight -inc 10")},
    { 0,                            XF86XK_MonBrightnessDown,                    spawn,   SHCMD("xbacklight -dec 10")},

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },

		/* Keep movemouse? */
    /* { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, */

		/* placemouse options, choose which feels more natural:
		 *    0 - tiled position is relative to mouse cursor
		 *    1 - tiled postiion is relative to window center
		 *    2 - mouse pointer warps to window center
		 *
		 * The moveorplace uses movemouse or placemouse depending on the floating state
		 * of the selected client. Set up individual keybindings for the two if you want
		 * to control these separately (i.e. to retain the feature to move a tiled window
		 * into a floating position).
		 */
		{ ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         ControlMask,    Button1,        dragmfact,      {0} },
    { ClkClientWin,         ControlMask,    Button3,        dragcfact,      {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabPrev,           0,              Button1,        movestack,      { .i = -1 } },
    { ClkTabNext,           0,              Button1,        movestack,      { .i = +1 } },
    { ClkTabClose,          0,              Button1,        killclient,     {0} },
};
