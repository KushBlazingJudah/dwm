/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]	      = "monospace:size=10";
static const char *fonts[]	    = { font };
static char normbgcolor[]	    = "#222222";
static char normbordercolor[]	    = "#444444";
static char normfgcolor[]	    = "#bbbbbb";
static char selfgcolor[]	    = "#eeeeee";
static char selbordercolor[]	    = "#005577";
static char selbgcolor[]	    = "#005577";
static char *colors[][3] = {
       /*		fg	     bg		  border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
//	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
//	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st-256color", NULL, NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static int deflayout = 0;
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[\\]",      dwindle },
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static char st9[]       = "st9";
static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]       = { "st", NULL };
static const char *lockcmd[]       = { "slock", NULL };
static const char *sysstatcmd[]       = { "sysstat", NULL };
static const char *brightinccmd[]  = { "brightness", "up", NULL };
static const char *brightdeccmd[]  = { "brightness", "down", NULL };
static const char *volupcmd[]     = { "volume", "up", NULL };
static const char *voldncmd[]     = { "volume", "down", NULL };
static const char *volmtcmd[]     = { "volume", "mute", NULL };
static const char *mpctogcmd[]     = { "mpc", "toggle", NULL };
static const char *mpcnexcmd[]     = { "mpc", "next", NULL };
static const char *mpcprecmd[]     = { "mpc", "prev", NULL };
static const char *rootmenucmd[]     = { "xrootmenu", NULL };

#define STCMD(cmd) {.v = (const char*[]){ "st", "-e", cmd, NULL }}
#define SWTCHCMD(cl, cmd) {.v = &(const struct swtch_t){.c = cl, .l = (const char*[]){cmd}}}
#define SWTCHSTCMD(cl, cmd) {.v = &(const struct swtch_t){.c = cl, .l = (const char*[]){"st", "-c", cl, "-e", cmd, NULL }}}
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },

static Key keys[] = {
	/* modifier			key	   function	   argument */

	/* Programs */
	{ MODKEY,			XK_grave,  spawn,	   {.v = sysstatcmd } },
	{ MODKEY,			XK_d,	   spawn,	   {.v = dmenucmd } },
	{ MODKEY,			XK_Return, spawn,	   {.v = termcmd } },
	{ MODKEY,			XK_x,	   spawn,	   {.v = lockcmd } },
	{ MODKEY,			XK_w,      swtch,	   SWTCHSTCMD("weechat", "weechat") },
	{ MODKEY|ShiftMask,		XK_w,      swtch,	   SWTCHCMD("firefox", "web") },
	{ MODKEY,			XK_e,	   swtch,	   SWTCHSTCMD("vim", "nvim") },
	{ MODKEY,			XK_n,	   swtch,	   SWTCHSTCMD("newsboat", "newsboat") },
	{ MODKEY|ShiftMask,		XK_m,	   swtch,	   SWTCHSTCMD("aerc", "aerc") },
	{ MODKEY,			XK_m,	   swtch,	   SWTCHSTCMD("ncmpcpp", "ncmpcpp") },

	/* Utils */
	{ MODKEY,			XK_slash,	spawn,	   {.v = brightinccmd } },
	{ MODKEY|ShiftMask,		XK_slash,	spawn,	   {.v = brightdeccmd } },
	{ MODKEY,			XK_backslash,	spawn,	   {.v = volmtcmd } },
	{ MODKEY,			XK_bracketleft,	spawn,	   {.v = voldncmd } },
	{ MODKEY,			XK_bracketright,spawn,	   {.v = volupcmd } },
	{ MODKEY,			XK_semicolon,	spawn,	   {.v = mpcnexcmd } },
	{ MODKEY|ShiftMask,		XK_semicolon,	spawn,	   {.v = mpcprecmd } },
	{ MODKEY,			XK_p,		spawn,	   {.v = mpctogcmd } },

	{ MODKEY,			XK_b,		togglebar, {0} },
	STACKKEYS(MODKEY,				focus)
	STACKKEYS(MODKEY|ShiftMask,			push)
	{ MODKEY|ShiftMask,		XK_h,		pushstack, {.i = 0 } },
	{ MODKEY|ShiftMask,		XK_l,		pushstack, {.i = -1 } },
	{ MODKEY|Mod1Mask,		XK_h,		focusstack,{.i = 0 } },
	{ MODKEY|Mod1Mask,		XK_l,		focusstack,{.i = -1 } },
	{ MODKEY,			XK_i,	   incnmaster,	   {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_i,	   incnmaster,	   {.i = -1 } },
	{ MODKEY,			XK_h,	   setmfact,	   {.f = -0.05} },
	{ MODKEY,			XK_l,	   setmfact,	   {.f = +0.05} },

//	{ MODKEY|ShiftMask,		XK_Return, zoom,	   {0} },

	{ MODKEY,			XK_t,	   setlayout,	   {.v = &layouts[0]} },
	{ MODKEY,			XK_f,	   togglefullscr,   {.v = &layouts[1]} },
//	{ MODKEY,			XK_f,	   setlayout,	   {.v = &layouts[1]} },
//	{ MODKEY,			XK_m,	   setlayout,	   {.v = &layouts[2]} },
	{ MODKEY,			XK_r,	   setlayout,	   {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,		XK_r,	   setlayout,	   {.v = &layouts[4]} },
	{ MODKEY,			XK_space,  setlayout,	   {0} },

	{ MODKEY|ShiftMask,		XK_q,	   killclient,	   {0} },
	{ MODKEY|ShiftMask,		XK_space,  togglefloating, {0} },
	{ MODKEY,			XK_s,	   togglesticky,   {0} },

	{ MODKEY,			XK_Tab,    view,	   {0} },
	{ MODKEY,			XK_0,	   view,	   {.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,	   tag,		   {.ui = ~0 } },
	{ MODKEY,			XK_comma,  focusmon,	   {.i = -1 } },
	{ MODKEY,			XK_period, focusmon,	   {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_comma,  tagmon,	   {.i = -1 } },
	{ MODKEY|ShiftMask,		XK_period, tagmon,	   {.i = +1 } },
	TAGKEYS(			XK_1,			   0)
	TAGKEYS(			XK_2,			   1)
	TAGKEYS(			XK_3,			   2)
	TAGKEYS(			XK_4,			   3)
	TAGKEYS(			XK_5,			   4)
	TAGKEYS(			XK_6,			   5)
	TAGKEYS(			XK_7,			   6)
	TAGKEYS(			XK_8,			   7)
	TAGKEYS(			XK_9,			   8)

	{ MODKEY|ShiftMask,		XK_e,	   quit,	   {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click		event mask	button		function	argument */
	{ ClkLtSymbol,		0,		Button1,	setlayout,	{0} },
	{ ClkLtSymbol,		0,		Button3,	setlayout,	{.v = &layouts[2]} },
	{ ClkStatusText,	0,		Button2,	spawn,		{.v = termcmd } },
	{ ClkClientWin,		MODKEY,		Button1,	movemouse,	{0} },
	{ ClkClientWin,		MODKEY,		Button2,	togglefloating, {0} },
	{ ClkClientWin,		MODKEY,		Button3,	resizemouse,	{0} },
	{ ClkTagBar,		0,		Button1,	view,		{0} },
	{ ClkTagBar,		0,		Button3,	toggleview,	{0} },
	{ ClkTagBar,		MODKEY,		Button1,	tag,		{0} },
	{ ClkTagBar,		MODKEY,		Button3,	toggletag,	{0} },
	{ ClkRootWin,		0,		Button3,	spawn,		{.v = rootmenucmd} },
};

