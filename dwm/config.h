#include <X11/XF86keysym.h>

static int showsystray                   = 1;         /* 是否显示托盘栏 */
static const int newclientathead         = 0;         /* 定义新窗口在栈顶还是栈底 */
static const unsigned int borderpx       = 2;         /* 窗口边框大小 */
static const unsigned int systraypinning = 1;         /* 托盘跟随的显示器 0代表不指定显示器 */
static const unsigned int systrayspacing = 1;         /* 托盘间距 */
static const unsigned int systrayspadding = 5;        /* 托盘和状态栏的间隙 */
static int gappi                         = 6;        /* 窗口与窗口 缝隙大小 Mine 5 */  
static int gappo                         = 6;        /* 窗口与边缘 缝隙大小 Mine 5 */
static const int _gappo                  = 12;        /* 窗口与窗口 缝隙大小 不可变 用于恢复时的默认值 */
static const int _gappi                  = 12;        /* 窗口与边缘 缝隙大小 不可变 用于恢复时的默认值 */
static const int vertpad                 = 5;         /* vertical padding of bar Mine 12 */
static const int sidepad                 = 5;         /* horizontal padding of bar Mine 12*/
static const int overviewgappi           = 5;        /* overview时 窗口与边缘 缝隙大小 Mine 5 */
static const int overviewgappo           = 24;        /* overview时 窗口与窗口 缝隙大小 Mine 24 */
static const int showbar                 = 1;         /* 是否显示状态栏 */
static const int topbar                  = 1;         /* 指定状态栏位置 0底部 1顶部 */
static const float mfact                 = 0.6;       /* 主工作区 大小比例 */
static const int   nmaster               = 1;         /* 主工作区 窗口数量 */
static const unsigned int snap           = 10;        /* 边缘依附宽度 */
static const unsigned int baralpha       = 0xc0;      /* 状态栏透明度 */
static const unsigned int borderalpha    = 0xdd;      /* 边框透明度 */
static const char *fonts[]               = { "ComicMono Nerd Font:style=medium:size=11", "monospace:size=11" };
static const char *colors[][3]           = {          /* 颜色设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { "#bbbbbb", "#333333", "#444444" },
    [SchemeSel] = { "#ffffff", "#37474F", "#42A5F5" },
    [SchemeSelGlobal] = { "#ffffff", "#37474F", "#FFC0CB" },
    [SchemeHid] = { "#dddddd", NULL, NULL },
    [SchemeSystray] = { NULL, "#7799AA", NULL },
    [SchemeUnderline] = { "#7799AA", NULL, NULL }, 
    [SchemeNormTag] = { "#bbbbbb", "#333333", NULL },
    [SchemeSelTag] = { "#eeeeee", "#333333", NULL },
    [SchemeBarEmpty] = { NULL, "#111111", NULL },
};
static const unsigned int alphas[][3]    = {          /* 透明度设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSel] = { OPAQUE, baralpha, borderalpha },
    [SchemeSelGlobal] = { OPAQUE, baralpha, borderalpha },
    [SchemeNormTag] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSelTag] = { OPAQUE, baralpha, borderalpha },
    [SchemeBarEmpty] = { NULL, 0xa0a, NULL },
    [SchemeStatusText] = { OPAQUE, 0x88, NULL },
};

/* 自定义脚本位置 */
// static const char *autostartscript = "~/scripts/autostart.sh";
static const char *statusbarscript = "$DWM/statusbar/statusbar.sh";

/* 自定义 scratchpad instance */
static const char scratchpadname[] = "scratchpad";

/* 自定义tag名称 */
/* 自定义特定实例的显示状态 */
//            ﮸  ﭮ 切
// 对应的tag序号以及快捷键:   0:1  1:2  2:3  3:4  4:5  5:9  6:c  7:m  8:0  9:w 10:l
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
static const Layout overviewlayout = { "舘",  overview };

/* 自定义布局 */
static const Layout layouts[] = {
    { "﬿",  tile },         /* 主次栈 */
    { "﩯",  magicgrid },    /* 网格 */
};

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define MODKEY Mod4Mask
#define Alt Mod1Mask
#define TAGKEYS(KEY, TAG, cmd) \
    { MODKEY,              KEY, view,       {.ui = 1 << TAG, .v = cmd} }, \
    { MODKEY|ShiftMask,    KEY, tag,        {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,  KEY, toggleview, {.ui = 1 << TAG} }, \

static const char *termcmd[]  = { "alacritty", NULL };

static Key keys[] = {
    /* modifier            key              function          argument */
    { MODKEY,              XK_equal,        togglesystray,    {0} },                     /* super +            |  切换 托盘栏显示状态 */

    // Mine
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

    { MODKEY,              XK_Tab,          focusstack,       {.i = +1} },               /* super tab          |  本tag内切换聚焦窗口 */
    { MODKEY|ShiftMask,    XK_Tab,          focusstack,       {.i = -1} },               /* super tab          |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_k,            focusstack,       {.i = -1} },               /* super up           |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_j,            focusstack,       {.i = +1} },               /* super down         |  本tag内切换聚焦窗口 */

    { MODKEY,              XK_Left,         viewtoleft,       {0} },                     /* super left         |  聚焦到左边的tag */
    { MODKEY,              XK_Right,        viewtoright,      {0} },                     /* super right        |  聚焦到右边的tag */
    { MODKEY|ShiftMask,    XK_Left,         tagtoleft,        {0} },                     /* super shift left   |  将本窗口移动到左边tag */
    { MODKEY|ShiftMask,    XK_Right,        tagtoright,       {0} },                     /* super shift right  |  将本窗口移动到右边tag */

    { MODKEY,              XK_a,            toggleoverview,   {0} },                     /* super a            |  显示所有tag 或 跳转到聚焦窗口的tag */

    { MODKEY|Alt,          XK_Left,         setmfact,         {.f = -0.01} },            /* super ,            |  缩小主工作区 */
    { MODKEY|Alt,          XK_Right,        setmfact,         {.f = +0.01} },            /* super .            |  放大主工作区 */

    { MODKEY,              XK_d,            hidewin,          {0} },                     /* super d            |  隐藏 窗口 */
    { MODKEY|ShiftMask,    XK_d,            restorewin,       {0} },                     /* super shift d      |  取消隐藏 窗口 */

    { MODKEY,              XK_s,            zoom,             {0} },                     /* super shift s      | 将当前聚焦窗口置为主窗口 */

    { MODKEY,              XK_q,            togglefloating,   {0} },                     /* super q            |  开启/关闭 聚焦目标的float模式 */
    { MODKEY|ShiftMask,    XK_q,            toggleallfloating,{0} },                     /* super shift q      |  开启/关闭 全部目标的float模式 */
    { MODKEY,              XK_f,            fullscreen,       {0} },                     /* super f            |  开启/关闭 全屏 */

    { MODKEY,              XK_g,            toggleglobal,     {0} },                     /* super g            |  开启/关闭 全局 */

    { MODKEY|ShiftMask,    XK_f,            togglebar,        {0} },                     /* super shift f      |  开启/关闭 状态栏 */
    { MODKEY,              XK_e,            incnmaster,       {.i = +1} },               /* super e            |  改变主工作区窗口数量 (1 2中切换) */


    { MODKEY,              XK_l,            focusmon,         {.i = +1} },               /* super l            |  光标移动到右边显示器 */
    { MODKEY,              XK_h,            focusmon,         {.i = -1} },               /* super h            |  光标移动到左边显示器 */
    { MODKEY|ShiftMask,    XK_l,            tagmon,           {.i = +1} },               /* super shift l      |  将聚焦窗口移动到右边显示器 */
    { MODKEY|ShiftMask,    XK_h,            tagmon,           {.i = -1} },               /* super shift h      |  将聚焦窗口移动到左边显示器 */

    { MODKEY|ShiftMask,    XK_w,            killclient,       {0} },                     /* super w            |  关闭窗口 */
    { MODKEY|ControlMask|ShiftMask,  XK_w,            forcekillclient,  {0} },                     /* super ctrl w       |  强制关闭窗口(处理某些情况下无法销毁的窗口) */
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
    { Alt,                                  XK_l,                         spawn,            SHCMD("rofi -show drun -theme mine -show-icons") },
    { Alt|ShiftMask,                        XK_l,                         spawn,            SHCMD("rofi -show run -theme mine -show-icons") },
    { Alt|ControlMask|ShiftMask,            XK_l,                         spawn,            SHCMD("rofi -show ssh -theme mine -show-icons") },

    { MODKEY,                               XK_F1,                        spawn,            SHCMD("xbacklight -get -5") },
    { MODKEY,                               XK_F2,                        spawn,            SHCMD("xbacklight -get +5") },
    { MODKEY,                               XK_F3,                        spawn,            SHCMD("amixer set Master toggle") },

    { Alt,                                  XK_s,                         spawn,            SHCMD("flameshot gui -p ~/Screenshot/shot") },
    { Alt|ShiftMask,                        XK_s,                         spawn,            SHCMD("flameshot gui -d 3000 -p ~/Screenshot/shot") },
    { Alt|ControlMask|ShiftMask,            XK_s,                         spawn,            SHCMD("flameshot full -p ~/Screenshot/full") },

    { Alt|ControlMask,                      XK_l,                         spawn,            SHCMD("slock") },
    { Alt,                                  XK_u,                         spawn,            SHCMD("pcmanfm") },
    { Alt,                                  XK_y,                         spawn,            SHCMD("/home/louis/.config/Build/Qv2ray/./Qv2ray") },
    { Alt,                                  XK_k,                         spawn,            SHCMD("screenkey -p fixed -g 50%x8%+25%-11%") },
    { Alt|ShiftMask,                        XK_k,                         spawn,            SHCMD("killall screenkey") },

    // --------------------------------------------------------------------


    /* spawn + SHCMD 执行对应命令(已下部分建议完全自己重新定义) */
    // { MODKEY,              XK_s,      togglescratch, SHCMD("st -t scratchpad -c float") },                      /* super s          | 打开scratch终端        */
    // { MODKEY,              XK_Return, spawn, SHCMD("alacritty") },                                                     /* super enter      | 打开st终端             */
    // { MODKEY,              XK_minus,  spawn, SHCMD("st -c global") },                                           /* super +          | 打开全局st终端         */
    // { MODKEY,              XK_space,  spawn, SHCMD("st -c float") },                                            /* super space      | 打开浮动st终端         */
    // { MODKEY,              XK_d,      spawn, SHCMD("rofi -show drun") },                             /* super d          | rofi: 执行run          */
    // { MODKEY|ShiftMask,    XK_d,      spawn, SHCMD("~/.config/Build/Qv2ray/.Qv2ray") },                            /* super shift d    | rofi: 执行drun         */
    // { MODKEY,              XK_p,      spawn, SHCMD("~/scripts/call_rofi.sh custom") },                          /* super p          | rofi: 执行自定义脚本   */
    // { MODKEY|ShiftMask,    XK_p,      spawn, SHCMD("~/scripts/call_rofi.sh window") },                          /* super shift p    | rofi: 执行window       */
    // { MODKEY,              XK_F1,     spawn, SHCMD("pcmanfm") },                                                /* super F1         | 文件管理器             */
    // { MODKEY,              XK_k,      spawn, SHCMD("~/scripts/blurlock.sh") },                                  /* super k          | 锁定屏幕               */
    // { MODKEY|ShiftMask,    XK_Up,     spawn, SHCMD("~/scripts/set_vol.sh up") },                                /* super shift up   | 音量加                 */
    // { MODKEY|ShiftMask,    XK_Down,   spawn, SHCMD("~/scripts/set_vol.sh down") },                              /* super shift down | 音量减                 */
    // { MODKEY|ShiftMask,    XK_a,      spawn, SHCMD("flameshot gui -c -p ~/Pictures/screenshots") },             /* super shift a    | 截图                   */
    // { MODKEY|ShiftMask,    XK_k,      spawn, SHCMD("~/scripts/screenkey.sh") },                                 /* super shift k    | 打开键盘输入显示       */
    // { MODKEY|ShiftMask,    XK_q,      spawn, SHCMD("kill -9 $(xprop | grep _NET_WM_PID | awk '{print $3}')") }, /* super shift q    | 选中某个窗口并强制kill */
    // { ShiftMask|ControlMask, XK_c,    spawn, SHCMD("xclip -o | xclip -selection c") },                          /* super shift c    | 进阶复制               */

    /* super key : 跳转到对应tag (可附加一条命令 若目标目录无窗口，则执行该命令) */
    /* super shift key : 将聚焦窗口移动到对应tag */
    /* key tag cmd */
    TAGKEYS(XK_1, 0,  0)
    TAGKEYS(XK_2, 1,  0)
    TAGKEYS(XK_3, 2,  0)
    TAGKEYS(XK_4, 3,  0)
    TAGKEYS(XK_5, 4,  0)
};
static Button buttons[] = {
    /* click               event mask       button            function       argument  */
    /* 点击窗口标题栏操作 */
    { ClkWinTitle,         0,               Button1,          hideotherwins, {0} },                                   // 左键        |  点击标题     |  隐藏其他窗口仅保留该窗口
    { ClkWinTitle,         0,               Button3,          togglewin,     {0} },                                   // 右键        |  点击标题     |  切换窗口显示状态
    /* 点击窗口操作 */
    { ClkClientWin,        MODKEY,          Button1,          movemouse,     {0} },                                   // super+左键  |  拖拽窗口     |  拖拽窗口
    { ClkClientWin,        MODKEY,          Button3,          resizemouse,   {0} },                                   // super+右键  |  拖拽窗口     |  改变窗口大小
    /* 点击tag操作 */
    { ClkTagBar,           0,               Button1,          view,          {0} },                                   // 左键        |  点击tag      |  切换tag
	{ ClkTagBar,           0,               Button3,          toggleview,    {0} },                                   // 右键        |  点击tag      |  切换是否显示tag
    { ClkTagBar,           MODKEY,          Button1,          tag,           {0} },                                   // super+左键  |  点击tag      |  将窗口移动到对应tag
    { ClkTagBar,           0,               Button4,          viewtoleft,    {0} },                                   // 鼠标滚轮上  |  tag          |  向前切换tag
	{ ClkTagBar,           0,               Button5,          viewtoright,   {0} },                                   // 鼠标滚轮下  |  tag          |  向后切换tag
    /* 点击状态栏操作 */
    { ClkStatusText,       0,               Button1,          clickstatusbar,{0} },                                   // 左键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal L
    { ClkStatusText,       0,               Button2,          clickstatusbar,{0} },                                   // 中键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal M
    { ClkStatusText,       0,               Button3,          clickstatusbar,{0} },                                   // 右键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal R
    { ClkStatusText,       0,               Button4,          clickstatusbar,{0} },                                   // 鼠标滚轮上  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal U
    { ClkStatusText,       0,               Button5,          clickstatusbar,{0} },                                   // 鼠标滚轮下  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal D
                                                                                                                      //
    /* 点击bar空白处 */
    { ClkBarEmpty,         0,               Button1,          spawn, SHCMD("~/scripts/call_rofi.sh window") },        // 左键        |  bar空白处    |  rofi 执行 window
    { ClkBarEmpty,         0,               Button3,          spawn, SHCMD("~/scripts/call_rofi.sh drun") },          // 右键        |  bar空白处    |  rofi 执行 drun
};
