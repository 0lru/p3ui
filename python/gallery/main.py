import asyncio
from p3ui import UserInterface, Window, Tab, TabItem, Style, em, ChildWindow, Button

import pathlib
from menu_bar import MenuBar
from tab_plots import TabPlots
from tab_widgets import TabWidgets
from tab_flexible import TabFlexible
from tab_styles import TabStyles
from tab_system import TabSystem

assets = pathlib.Path(__file__).parent.joinpath('assets').absolute()


def set_default_font(ui):
    ui.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 20)
    ui.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 20)


ui = UserInterface(menu_bar=MenuBar())
set_default_font(ui)
window = Window(user_interface=ui)

tab_plots = TabPlots()
tab_system = TabSystem(window)

ui.content = Tab(
    style=Style(padding=(1.5 | em, 0.5 | em)),
    children=[
        TabItem("Flexible", content=TabFlexible()),
        TabItem("Widgets", content=TabWidgets(ui, assets)),
        TabItem("Plots", content=tab_plots),
        TabItem("Styles", content=TabStyles(ui)),
        TabItem("System", content=tab_system)
    ]
)


async def main():
    asyncio.get_event_loop().create_task(tab_system.update())
    while not window.closed:
        tab_plots.update()
        window.frame()
        await asyncio.sleep(0)
    print('shutting down')


asyncio.run(main())
# sync would be:
# window.loop(on_frame=lambda _: plots.update())
