from p3ui import UserInterface, Window, Tab, TabItem, Style, em

import pathlib
from menu_bar import MenuBar
from tab_plots import TabPlots
from tab_widgets import TabWidgets
from tab_flexible import TabFlexible
from tab_styles import TabStyles
from tab_system import TabSystem

assets = pathlib.Path(__file__).parent.joinpath('assets').absolute()

ui = UserInterface(menu_bar=MenuBar())
ui.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 20)
ui.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 24)

window = Window(user_interface=ui)

plots = TabPlots()
ui.content = Tab(
    style=Style(padding=(1.5 | em, 0.5 | em)),
    children=[
        TabItem("Flexible", content=TabFlexible()),
        TabItem("Widgets", content=TabWidgets(ui, assets)),
        TabItem("Plots", content=plots),
        TabItem("Styles", content=TabStyles(ui)),
        TabItem("System", content=TabSystem(window))
    ]
)

window.loop(on_frame=lambda _: plots.update())
