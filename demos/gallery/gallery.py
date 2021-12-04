import pathlib
import asyncio

from p3ui import *

from material_icons import MaterialIcons
from menu_bar import MenuBar
from tab_plots import TabPlots
from tab_widgets import TabWidgets
from tab_layout import TabLayout
from tab_styles import TabStyles
from tab_system import TabSystem
from tab_icons import TabIcons
from tab_tables import TabTables


class Gallery(UserInterface):

    def __init__(self, window):
        super().__init__(
            menu_bar=MenuBar()
        )
        tab_plots = TabPlots()
        tab_system = TabSystem(window)

        assets = pathlib.Path(__file__).parents[1].joinpath('assets').absolute()
        self.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 20)
        self.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 20)
        self.content = Tab(
            padding=(1.5 | em, 0.5 | em),

            children=[
                TabItem(f'{MaterialIcons.VerticalAlignCenter} Layout', content=TabLayout()),
                TabItem(f'{MaterialIcons.Widgets} Widgets', content=TabWidgets(self, assets)),
                TabItem(f'{MaterialIcons.LightbulbOutline} Icons', content=TabIcons()),
                TabItem(f'{MaterialIcons.MultilineChart} Plots', content=tab_plots),
                TabItem(f'{MaterialIcons.BorderOuter} Styles', content=TabStyles(self)),
                TabItem(f'{MaterialIcons.Settings} System', content=tab_system),
                TabItem(f'{MaterialIcons.Tab} Tables', content=TabTables())
            ])

        self.t1 = asyncio.get_event_loop().create_task(tab_system.update())
        self.t2 = asyncio.get_event_loop().create_task(tab_plots.update())

    def shutdown(self):
        self.t1.cancel()
        self.t2.cancel()
