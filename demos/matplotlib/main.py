from p3ui import *
import asyncio
import matplotlib
import pathlib
from bar_chart import BarChart
from filled_chart import FilledChart
from gradient_chart import GradientChart
from line_styles import LineStyles
from mark_every import MarkEvery

font_size_px = 13
#
# matplotlib expects points. we use default dpi and compute 13px in points..
matplotlib.rcParams.update({'font.size': pixels_to_points(font_size_px)})


async def main():
    window = Window(title='matplotlib')
    window.position = (50, 50)
    window.size = (1000, 900)

    user_interface = UserInterface(content=Column(
        width=(font_size_px * 3 | em, 0, 0), height=(font_size_px * 4 | em, 0, 0),
        children=[
            Text(f'ImGui-Text @ {font_size_px}px'),
            Row(
                padding=(0 | px, 0 | px),
                height=(1 | px, 1, 1),
                children=[BarChart(), FilledChart()]),
            Row(
                padding=(0 | px, 0 | px),
                height=(1 | px, 2, 1),
                children=[
                    LineStyles(),
                    Column(
                        padding=(0 | px, 0 | px),
                        children=[
                            GradientChart(),
                            MarkEvery()
                        ]
                    )
                ]),
        ]
    ))

    assets = pathlib.Path(__file__).parents[1].joinpath('assets').absolute()
    user_interface.load_font(assets.joinpath("DroidSans.ttf").as_posix(), font_size_px)
    user_interface.merge_font(assets.joinpath("MaterialIcons-Regular.ttf").as_posix(), font_size_px)

    await window.serve(user_interface)


asyncio.run(main())
