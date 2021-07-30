import asyncio
import traceback
from p3ui import UserInterface, Window, Tab, TabItem, Style, em, ChildWindow, Button, Builder


def make_template(builder):
    template = """
    <Layout width="2em" justify-content="{get}" >
      <Button width="5em"/>
    </Layout>
    """


import pathlib
from menu_bar import MenuBar
from tab_plots import TabPlots
from tab_widgets import TabWidgets
from tab_layout import TabLayout
from tab_styles import TabStyles
from tab_system import TabSystem
from tab_icons import TabIcons

assets = pathlib.Path(__file__).parent.joinpath('assets').absolute()


def set_default_font(ui):
    ui.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 16)
    ui.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 16)


ui = UserInterface(menu_bar=MenuBar())
set_default_font(ui)
window = Window(user_interface=ui)

tab_plots = TabPlots()
tab_system = TabSystem(window)

ui.content = Tab(
    padding=(1.5 | em, 0.5 | em),
    children=[
        TabItem("Icons", content=TabIcons()),
        TabItem("Layout", content=TabLayout()),
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
exit(0)


# sync would be:
# window.loop(on_frame=lambda _: plots.update())


def test():
    print('clicked')


builder = Builder()

node = builder.build(f"""
<Layout 
    justify-content="space-around" 
    align-items="start">
    <Button 
        label="NoName" 
        on_click="{builder.bind(test)}" 
        width="100px 2 3"/>
</Layout>
""")

ui = UserInterface(content=node)
window = Window(user_interface=ui)


async def main():
    while not window.closed:
        window.frame()
        await asyncio.sleep(0)


asyncio.run(main())

exit(0)
