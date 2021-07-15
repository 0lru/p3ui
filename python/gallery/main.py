from oogui import Context, Window, Tab, TabItem, Style, em
import pathlib
from plots import Plots
from widgets import Widgets
from flexibles import Flexibles
from tables import Tables
from style_editor import StyleEditor
from main_menu_bar import MainMenuBar

#
# create oogui context. the window must be created first, else texture loading etc. will not work
context = Context()
window = Window(context)

#
# get asset folder
assets = pathlib.Path(__file__).parent.joinpath('assets').absolute()

#
# load main font, merge icon font
context.default_font = context.load_font(assets.joinpath("DroidSans.ttf").as_posix(), 20)
context.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), 24)

#
# merge examples to tab
plots = Plots()
window.content = Tab(style=Style(padding=(1 | em, 0.5 | em)))
# window.content.add(TabItem("Tables (ImGui)", content=Tables()))
window.content.add(TabItem("Flexible", content=Flexibles()))
window.content.add(TabItem("Widgets", content=Widgets(window, assets)))
window.content.add(TabItem("Plots", content=plots))
window.content.add(TabItem("Style Editor", content=StyleEditor(window)))

#
# create a menu
window.menu_bar = MainMenuBar()

#
# enter main loop
window.loop(on_frame=lambda _: plots.update())
