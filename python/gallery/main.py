from p3ui import UserInterface, Window, Tab, TabItem, Style, em, px
import p3ui as p3

import pathlib
from plots import Plots
from widgets import Widgets
from flexibles import Flexibles
from style_editor import StyleEditor
from main_menu_bar import MainMenuBar

assets = pathlib.Path(__file__).parent.joinpath('assets').absolute()

ui = UserInterface(menu_bar=MainMenuBar())
ui.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 20)
ui.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 24)
plots = Plots()
window = Window(user_interface=ui)


def set_video_mode(video_mode):
    print(f'set mode {video_mode.width}x{video_mode.height}')
    window.video_mode = video_mode


def set_windowed():
    window.video_mode = None


display_menu = p3.Menu('View')
ui.menu_bar.add(display_menu)
display_menu.add(p3.MenuItem('Windowed', on_click=set_windowed))
for index, monitor in enumerate(window.monitors()):
    monitor_menu = p3.Menu(f'Monitor {index} ')
    display_menu.add(monitor_menu)
    for mode in monitor.modes:
        monitor_menu.add(p3.MenuItem(
            f'  {mode.width}x{mode.height} {mode.hz}Hz',
            on_click=lambda x=mode: set_video_mode(x)))

ui.content = Tab(
    style=Style(padding=(1.5 | em, 0.5 | em)),
    children=[
        TabItem("Flexible", content=Flexibles()),
        TabItem("Widgets", content=Widgets(ui, assets)),
        TabItem("Plots", content=plots),
        TabItem("Style", content=StyleEditor(ui))
    ]
)

#
# enter main loop
window.loop(on_frame=lambda _: plots.update())
