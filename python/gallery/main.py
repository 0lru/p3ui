from p3ui import UserInterface, Window, Tab, TabItem, Style, em

import pathlib
from plots import Plots
from widgets import Widgets
from flexibles import Flexibles
from style_editor import StyleEditor
from main_menu_bar import MainMenuBar
from view_options import ViewOptions

assets = pathlib.Path(__file__).parent.joinpath('assets').absolute()

ui = UserInterface(menu_bar=MainMenuBar())
ui.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 20)
ui.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 24)

window = Window(user_interface=ui)

plots = Plots()
ui.content = Tab(
    style=Style(padding=(1.5 | em, 0.5 | em)),
    children=[
        TabItem("Flexible", content=Flexibles()),
        TabItem("Widgets", content=Widgets(ui, assets)),
        TabItem("Plots", content=plots),
        TabItem("Style", content=StyleEditor(ui)),
        TabItem("View Options", content=ViewOptions(window))
    ]
)

window.loop(on_frame=lambda _: plots.update())
