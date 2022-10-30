#include <X11/XF86keysym.h>

static int showsystray                   = 1;         /* 是否显示托盘栏 */
static const int newclientathead         = 0;         /* 定义新窗口在栈顶还是栈底 */
static const unsigned int borderpx       = 2;         /* 窗口边框大小 */
static const unsigned int systraypinning = 1;         /* 托盘跟随的显示器 0代表不指定显示器 */
static const unsigned int systrayspacing = 1;         /* 托盘间距 */
// static int gappi                         = 12;        /* 窗口与窗口 缝隙大小 */
// static int gappo                         = 12;        /* 窗口与边缘 缝隙大小 */
static int gappi                         = 3;        /* 窗口与窗口 缝隙大小 */
static int gappo                         = 8;        /* 窗口与边缘 缝隙大小 */
static const int _gappo                  = 12;        /* 窗口与窗口 缝隙大小 不可变 用于恢复时的默认值 */
static const int _gappi                  = 12;        /* 窗口与边缘 缝隙大小 不可变 用于恢复时的默认值 */
static const int overviewgappi           = 8;        /* overview时 窗口与边缘 缝隙大小 */
static const int overviewgappo           = 20;        /* overview时 窗口与窗口 缝隙大小 */
static const int showbar                 = 1;         /* 是否显示状态栏 */
static const int topbar                  = 1;         /* 指定状态栏位置 0底部 1顶部 */
static const float mfact                 = 0.55;       /* 主工作区 大小比例 */
static const int   nmaster               = 1;         /* 主工作区 窗口数量 */
static const unsigned int snap           = 8;        /* 边缘依附宽度 */
static const unsigned int baralpha       = 0xc0;      /* 状态栏透明度 */
static const unsigned int borderalpha    = 0xdd;      /* 边框透明度 */
static const char *fonts[]               = { "Sarasa Mono SC Nerd:size=13",
                                             "Sarasa Mono SC Nerd:size=13:type=Regular:antialias=true:autohint=true" };
static const char *colors[][3]           = { [SchemeNorm] = { "#bbbbbb", "#333333", "#444444" }, [SchemeSel] = { "#ffffff", "#37474F", "#42A5F5" }, [SchemeHid] = { "#dddddd", NULL, NULL }, [SchemeSystray] = { "#7799AA", "#7799AA", "#7799AA" }, [SchemeUnderline] = { "#7799AA", "#7799AA", "#7799AA" } };
static const unsigned int alphas[][3]    = { [SchemeNorm] = { OPAQUE, baralpha, borderalpha }, [SchemeSel] = { OPAQUE, baralpha, borderalpha } };


/* 自定义tag名称 */
/* 自定义特定实例的显示状态 */
//            ﮸ 
static const char *tags[] = { "", "", "", "", ""};
static const Rule rules[] = {
    /* class                 instance              title             tags mask     isfloating   noborder  monitor */
    { NULL,                  NULL,                "broken",          0,            1,           0,        -1 },
    { NULL,                  NULL,                "图片查看",        0,            1,           0,        -1 },
    { NULL,                  NULL,                "图片预览",        0,            1,           0,        -1 },
    { NULL,                  NULL,                "crx_",            0,            1,           0,        -1 },
    {"chrome",               NULL,                 NULL,             1 << 9,       0,           0,        -1 },
    {"Chromium",             NULL,                 NULL,             1 << 9,       0,           0,        -1 },
    {"float",                NULL,                 NULL,             0,            1,           0,        -1 },
    {"flameshot",            NULL,                 NULL,             0,            1,           0,        -1 },
};
static const char *overviewtag = "OVERVIEW";
static const Layout overviewlayout = { "",  overview };

/* 自定义布局 */
static const Layout layouts[] = {
    { "﬿",  tile },         /* 主次栈 */
    { "",  magicgrid },    /* 网格 */
};

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define MODKEY Mod4Mask
#define Alt Mod1Mask
#define TAGKEYS(KEY, TAG, cmd1, cmd2) \
    { MODKEY,              KEY, view,       {.ui = 1 << TAG, .v = cmd1} }, \
    { MODKEY|ShiftMask,    KEY, tag,        {.ui = 1 << TAG, .v = cmd2} }, \
    { MODKEY|ControlMask,  KEY, toggleview, {.ui = 1 << TAG} }, \

static const char *termcmd[]  = { "alacritty", NULL };

static Key keys[] = {
    /* modifier            key              function          argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

    { MODKEY,              XK_equal,        togglesystray,    {0} },                     /* super +            |  切换 托盘栏显示状态 */

    { MODKEY,              XK_Tab,          focusstack,       {.i = +1} },               /* super tab          |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_k,            focusstack,       {.i = -1} },               /* super up           |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_j,            focusstack,       {.i = +1} },               /* super down         |  本tag内切换聚焦窗口 */

    { MODKEY,              XK_Left,         viewtoleft,       {0} },                     /* super left         |  聚焦到左边的tag */
    { MODKEY,              XK_Right,        viewtoright,      {0} },                     /* super right        |  聚焦到右边的tag */
    { MODKEY|ShiftMask,    XK_Left,         tagtoleft,        {0} },                     /* super shift left   |  将本窗口移动到左边tag */
    { MODKEY|ShiftMask,    XK_Right,        tagtoright,       {0} },                     /* super shift right  |  将本窗口移动到右边tag */

    { MODKEY,              XK_a,            toggleoverview,   {0} },                     /* super a            |  显示所有tag 或 跳转到聚焦窗口的tag */

    { MODKEY|Alt,          XK_Left,         setmfact,         {.f = -0.01} },            /* super ,            |  缩小主工作区 */
    { MODKEY|Alt,          XK_Right,        setmfact,         {.f = +0.01} },            /* super .            |  放大主工作区 */

    { MODKEY,              XK_g,            hidewin,          {0} },                     /* super h            |  隐藏 窗口 */
    { MODKEY|ShiftMask,    XK_g,            restorewin,       {0} },                     /* super shift h      |  取消隐藏 窗口 */

    { MODKEY|ShiftMask,    XK_Return,       zoom,             {0} },                     /* super shift enter  |  将当前聚焦窗口置为主窗口 */

    { MODKEY,              XK_q,            togglefloating,   {0} },                     /* super t            |  开启/关闭 聚焦目标的float模式 */
    { MODKEY|ShiftMask,    XK_q,            toggleallfloating,{0} },                     /* super shift t      |  开启/关闭 全部目标的float模式 */
    { MODKEY,              XK_f,            fullscreen,       {0} },                     /* super f            |  开启/关闭 全屏 */
    { MODKEY|ShiftMask,    XK_f,            togglebar,        {0} },                     /* super shift f      |  开启/关闭 状态栏 */
    { MODKEY,              XK_e,            incnmaster,       {.i = +1} },               /* super e            |  改变主工作区窗口数量 (1 2中切换) */

    { MODKEY,              XK_l,            focusmon,         {.i = +1} },               /* super b            |  光标移动到右边显示器 */
    { MODKEY,              XK_h,            focusmon,         {.i = -1} },               /* super b            |  光标移动到左边显示器 */
    { MODKEY|ShiftMask,    XK_l,            tagmon,           {.i = +1} },               /* super shift b      |  将聚焦窗口移动到右边显示器 */
    { MODKEY|ShiftMask,    XK_h,            tagmon,           {.i = -1} },               /* super shift b      |  将聚焦窗口移动到左边显示器 */

    { MODKEY|ShiftMask,    XK_w,            killclient,       {0} },                     /* super q            |  关闭窗口 */
    { MODKEY|Alt,          XK_F12,          quit,             {0} },                     /* super ctrl f12     |  退出dwm */

	{ MODKEY|ShiftMask,    XK_space,        selectlayout,     {.v = &layouts[1]} },      /* super shift space  |  切换到网格布局 */
	{ MODKEY,              XK_o,            showonlyorall,    {0} },                     /* super o            |  切换 只显示一个窗口 / 全部显示 */

    { MODKEY|ControlMask,  XK_equal,        setgap,           {.i = -6} },               /* super ctrl up      |  窗口增大 */
    { MODKEY|ControlMask,  XK_minus,        setgap,           {.i = +6} },               /* super ctrl down    |  窗口减小 */
    { MODKEY|ControlMask,  XK_space,        setgap,           {.i = 0} },                /* super ctrl space   |  窗口重置 */

    { MODKEY|ControlMask,  XK_k,           movewin,          {.ui = UP} },              /* super ctrl up      |  移动窗口 */
    { MODKEY|ControlMask,  XK_j,           movewin,          {.ui = DOWN} },            /* super ctrl down    |  移动窗口 */
    { MODKEY|ControlMask,  XK_h,           movewin,          {.ui = LEFT} },            /* super ctrl left    |  移动窗口 */
    { MODKEY|ControlMask,  XK_l,           movewin,          {.ui = RIGHT} },           /* super ctrl right   |  移动窗口 */

    { MODKEY|Alt,     XK_k,           resizewin,        {.ui = V_REDUCE} },        /* super ctrl up      |  调整窗口 */
    { MODKEY|Alt,     XK_j,           resizewin,        {.ui = V_EXPAND} },        /* super ctrl down    |  调整窗口 */
    { MODKEY|Alt,     XK_h,           resizewin,        {.ui = H_REDUCE} },        /* super ctrl left    |  调整窗口 */
    { MODKEY|Alt,     XK_l,           resizewin,        {.ui = H_EXPAND} },        /* super ctrl right   |  调整窗口 */

    /* spawn + SHCMD 执行对应命令 */
    { Alt,                                  XK_l,                         spawn,            SHCMD("rofi options -theme mine -show drun -show-icons") },
    { Alt|ShiftMask,                        XK_l,                         spawn,            SHCMD("rofi options -theme mine -show run -show-icons") },
    { Alt|ControlMask|ShiftMask,            XK_l,                         spawn,            SHCMD("rofi options -theme mine -show ssh -show-icons") },

    { MODKEY,                               XK_F3,                        spawn,            SHCMD("amixer set Master toggle") },
    { MODKEY,                               XK_F1,                        spawn,            SHCMD("xbacklight -get -5") },
    { MODKEY,                               XK_F2,                        spawn,            SHCMD("xbacklight -get +5") },

    { Alt,                                  XK_s,                         spawn,            SHCMD("flameshot gui -p ~/Screenshot/shot") },
    { Alt|ShiftMask,                        XK_s,                         spawn,            SHCMD("flameshot gui -d 3000 -p ~/Screenshot/shot") },
    { Alt|ControlMask|ShiftMask,            XK_s,                         spawn,            SHCMD("flameshot full -p ~/Screenshot/full") },

    { Alt|ControlMask,                      XK_l,                         spawn,            SHCMD("slock") },
    { Alt,                                  XK_u,                         spawn,            SHCMD("thunar") },
    { Alt,                                  XK_y,                         spawn,            SHCMD("/opt/./Qv2ray") },
    { Alt,                                  XK_k,                         spawn,            SHCMD("screenkey") },
    { Alt|ShiftMask,                        XK_k,                         spawn,            SHCMD("killall screenkey") },
    // { MODKEY|ShiftMask,    XK_q,            spawn,            SHCMD("~/scripts/app-starter.sh killw") },
    // { MODKEY,              XK_minus,        spawn,            SHCMD("~/scripts/app-starter.sh fst") },
    // { MODKEY,              XK_Return,       spawn,            SHCMD("~/scripts/app-starter.sh st") },
    // { MODKEY|ShiftMask,    XK_a,            spawn,            SHCMD("~/scripts/app-starter.sh flameshot") },
    // { MODKEY,              XK_d,            spawn,            SHCMD("~/scripts/app-starter.sh rofi") },
    // { MODKEY,              XK_space,        spawn,            SHCMD("~/scripts/app-starter.sh rofi_window") },
    // { MODKEY,              XK_p,            spawn,            SHCMD("~/scripts/app-starter.sh rofi_p") },
    // { MODKEY|ShiftMask,    XK_k,            spawn,            SHCMD("~/scripts/app-starter.sh screenkey") },
    // { MODKEY,              XK_k,            spawn,            SHCMD("~/scripts/app-starter.sh blurlock") },
    // { MODKEY,              XK_F1,           spawn,            SHCMD("~/scripts/app-starter.sh filemanager") },
    // { MODKEY|ShiftMask,    XK_Up,           spawn,            SHCMD("~/scripts/app-starter.sh set_vol up &") },
    // { MODKEY|ShiftMask,    XK_Down,         spawn,            SHCMD("~/scripts/app-starter.sh set_vol down &") },
    // { MODKEY,              XK_j,            spawn,            SHCMD("~/scripts/app-starter.sh robot") },
    // { MODKEY|ShiftMask,    XK_j,            spawn,            SHCMD("~/scripts/app-starter.sh robot onlyclick") },
    // { ShiftMask|ControlMask, XK_c,          spawn,            SHCMD("xclip -o | xclip -selection c") },

    /* super key : 跳转到对应tag */
    /* super shift key : 将聚焦窗口移动到对应tag */
    /* 若跳转后的tag无窗口且附加了cmd1或者cmd2就执行对应的cmd */
    /* key tag cmd1 cmd2 */
    TAGKEYS(XK_1, 0,  0,  0)
    TAGKEYS(XK_2, 1,  0,  0)
    TAGKEYS(XK_3, 2,  0,  0)
    TAGKEYS(XK_4, 3,  0,  0)
    TAGKEYS(XK_5, 4,  0,  0)
};
static Button buttons[] = {
    /* click               event mask       button            function       argument  */
	// { ClkStatusText,       0,               Button1,          spawn,         SHCMD("~/scripts/app-starter.sh fst") }, // 左键        |  点击状态栏   |  打开float st
    { ClkWinTitle,         0,               Button1,          hideotherwins, {0} },                                   // 左键        |  点击标题     |  隐藏其他窗口仅保留该窗口
    { ClkWinTitle,         0,               Button3,          togglewin,     {0} },                                   // 右键        |  点击标题     |  切换窗口显示状态
    { ClkTagBar,           0,               Button1,          view,          {0} },                                   // 左键        |  点击tag      |  切换tag
	{ ClkTagBar,           0,               Button3,          toggleview,    {0} },                                   // 右键        |  点击tag      |  切换是否显示tag
    { ClkClientWin,        MODKEY,          Button1,          movemouse,     {0} },                                   // super+左键  |  拖拽窗口     |  拖拽窗口
    { ClkClientWin,        MODKEY,          Button3,          resizemouse,   {0} },                                   // super+右键  |  拖拽窗口     |  改变窗口大小
    { ClkTagBar,           MODKEY,          Button1,          tag,           {0} },                                   // super+左键  |  点击tag      |  将窗口移动到对应tag
};
