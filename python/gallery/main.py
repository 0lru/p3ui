import asyncio
from p3ui import *

import pathlib
from menu_bar import MenuBar
from tab_plots import TabPlots
from tab_widgets import TabWidgets
from tab_layout import TabLayout
from tab_styles import TabStyles
from tab_system import TabSystem
from tab_icons import TabIcons

from material_icons import MaterialIcons

assets = pathlib.Path(__file__).parent.joinpath('assets').absolute()


def set_default_font(ui):
    ui.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 18)
    ui.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 18)


ui = UserInterface(menu_bar=MenuBar())
set_default_font(ui)
window = Window(user_interface=ui)

tab_plots = TabPlots()
tab_system = TabSystem(window)

ui.content = Layout(
    direction=Direction.Vertical,
    children=[
        Tab(
            padding=(1.5 | em, 0.5 | em),
            children=[
                TabItem("Icons", content=TabIcons()),
                TabItem("Layout", content=TabLayout()),
                TabItem("Widgets", content=TabWidgets(ui, assets)),
                TabItem("Plots", content=tab_plots),
                TabItem("Styles", content=TabStyles(ui)),
                TabItem("System", content=tab_system)
            ]),
        Layout(
            height=(None, 0, 0),
            direction=Horizontal,
            align_items=Alignment.Center,
            justify_content=Justification.End,
            children=[Text(f'{MaterialIcons.Timer}')]
        )
    ])


async def main():
    asyncio.get_event_loop().create_task(tab_system.update())
    while not window.closed:
        tab_plots.update()
        window.frame()
        await asyncio.sleep(0)
    print('shutting down')


asyncio.run(main())
exit(0)
