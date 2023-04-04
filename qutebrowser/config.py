# TODO: Separate sections into different files

import os

if 'NODE_PATH' in os.environ:
    os.environ['NODE_PATH'] += os.pathsep + '/usr/local/lib/node_modules'
else:
    os.environ['NODE_PATH'] = os.pathsep + '/usr/local/lib/node_modules'
os.environ['NODE_PATH'] += os.pathsep + '/Users/leosol/.nvm/versions/node/v16.4.2/lib/node_modules'

os.environ['PATH']                      += os.pathsep + '/usr/local/bin'
os.environ['PATH']                      += os.pathsep + '/usr/bin'

# TODO create toggle for diagnostics
# c.colors.webpage.darkmode.enabled        = True
c.colors.webpage.darkmode.policy.images  = 'never'
c.colors.webpage.darkmode.policy.page    = 'always'
c.colors.webpage.darkmode.grayscale.all  = False
c.colors.webpage.preferred_color_scheme  = 'dark'

c.content.autoplay                       = False
c.content.blocking.method                = 'adblock'

c.editor.command                         = ['alacritty', '--single-instance', 'nvim', '{}']

c.fonts.default_family                   = 'MesloLGS NF'
c.fonts.default_size                     = '11pt'
c.fonts.downloads                        = 'MesloLGS NF'
c.fonts.hints                            = 'default_size default_family'

c.hints.chars                            = 'arstneiogmdh'

c.scrolling.bar                          = 'always'
c.scrolling.smooth                       = True

c.session.lazy_restore                   = True

c.tabs.background                        = True
c.tabs.max_width                         = -1
c.tabs.new_position.unrelated            = 'next'
c.tabs.position                          = 'left'
c.tabs.select_on_remove                  = 'next'
c.tabs.show                              = 'always'
c.tabs.show_switching_delay              = 5000

# c.tabs.title.format                      = '{relative_index}| {audio}{private}{current_title}'
# c.tabs.title.format_pinned               = '{relative_index}* {audio}{private}{current_title}'
c.tabs.title.format                      = '{index}| {audio}{private}{current_title}'
c.tabs.title.format_pinned               = '{index}* {audio}{private}{current_title}'

c.url.searchengines                      = {'DEFAULT': 'https://www.google.com/search?q={}', 'g': 'https://www.google.com/search?q={}', 'ddg': 'https://www.duckduckgo.com/?q={}', 'y': 'https://www.youtube.com/results?search_query={}'}

c.window.hide_decoration                 = False

c.url.default_page = 'about:blank'
c.url.start_pages = ['duckduckgo.com']


# 滚动条样式
# A list of user stylesheet filenames to use.
c.content.user_stylesheets = "user.css"


#########
# Binds #
#########

# scrolling
config.bind('<Ctrl+d>', 'scroll-px 0 100')
config.bind('<Ctrl+u>', 'scroll-px 0 -100')

# paste
config.bind('PP', 'open -t -- {clipboard}')

# tab
config.bind('J', 'tab-next')
config.bind('K', 'tab-prev')
config.bind('<Ctrl+Shift+j>', 'tab-move +')
config.bind('<Ctrl+Shift+k>', 'tab-move -')
# config.bind('cn', 'tab-only -p -P keep')
# config.bind('cp', 'tab-only -n -P keep')
# config.bind('ce', 'config-edit')
config.bind('p', 'tab-pin;;tab-move 1')
config.bind('t', 'set-cmd-text -s :tab-select')
config.bind('ss', 'set-cmd-text -s :session-save')
config.bind('sl', 'set-cmd-text -s :session-load --clear')

# translate
# config.bind(',t', 'hint userscript link translate')
# config.bind(',T', 'hint userscript all translate --text')
# config.bind('<Ctrl+T>', 'spawn --userscript translate')
config.bind('<Alt+Shift+q>', 'spawn --userscript translate --text')

# add bookmark
# config.bind('aa', 'set-cmd-text -s :quickmark-add {url} {title}')
config.bind('aa', 'set-cmd-text -s :quickmark-add {url} ')

config.bind(',b', 'config-cycle content.blocking.enabled true false')

# open web
config.bind('wk', 'open -t https://wiki.archlinux.org/title/qutebrowser')
config.bind('gh', 'open -t https://github.com/xanmodlouis?tab=repositories')

config.bind('sc', 'config-source')
config.bind('<Space>r', 'spawn --userscript readability-js')

# toggle configs
# config.bind('xx', 'config-cycle window.hide_decoration true false')
config.bind('<Alt+a>', 'config-cycle statusbar.show always never;; config-cycle tabs.show always never')
config.bind('xb', 'config-cycle statusbar.show always never')

config.load_autoconfig(False)

config.set('content.cookies.accept', 'all', 'chrome-devtools://*')
config.set('content.cookies.accept', 'all', 'devtools://*')
config.set('content.desktop_capture', True, 'https://app.chime.aws')
config.set('content.headers.accept_language', '', 'https://matchmaker.krunker.io/*')
config.set('content.headers.user_agent', 'Mozilla/5.0 ({os_info}) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99 Safari/537.36', 'https://*.slack.com/*')
config.set('content.headers.user_agent', 'Mozilla/5.0 ({os_info}) AppleWebKit/{webkit_version} (KHTML, like Gecko) {upstream_browser_key}/{upstream_browser_version} Safari/{webkit_version} Edg/{upstream_browser_version}', 'https://accounts.google.com/*')
config.set('content.headers.user_agent', 'Mozilla/5.0 ({os_info}) AppleWebKit/{webkit_version} (KHTML, like Gecko) {upstream_browser_key}/{upstream_browser_version} Safari/{webkit_version}', 'https://web.whatsapp.com/')

config.set('content.images', True, 'chrome-devtools://*')
config.set('content.images', True, 'devtools://*')
config.set('content.javascript.enabled', True, 'chrome-devtools://*')
config.set('content.javascript.enabled', True, 'chrome://*/*')
config.set('content.javascript.enabled', True, 'devtools://*')
config.set('content.javascript.enabled', True, 'qute://*/*')
config.set('content.media.audio_capture', True, 'https://app.chime.aws')
config.set('content.media.audio_video_capture', True, 'https://app.chime.aws')
config.set('content.media.video_capture', True, 'https://app.chime.aws')
config.set('content.register_protocol_handler', True, 'https://mail.google.com?extsrc=mailto&url=%25s')

config.source('themes/onedark.py')
