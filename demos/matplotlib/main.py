from p3ui import *
import asyncio
from bar_chart import BarChart
from filled_chart import FilledChart
from gradient_chart import GradientChart
import matplotlib

imgui_font_size = 13
#
# matplotlib expects points. we use default dpi and compute 13px in points..
matplotlib.rcParams.update({'font.size': pixels_to_points(imgui_font_size)})


async def main():
    window = Window(title='matplotlib')
    window.position = (50, 50)
    window.size = (800, 800)

    await window.serve(UserInterface(
        content=ScrollArea(content=Column(
            width=(90 | em, 0, 0),
            height=(80 | em, 0, 0),
            children=[
                Text('ImGui-Text @ 13px'),
                Row(padding=(0 | px, 0 | px), children=[BarChart(), FilledChart()]),
                Row(padding=(0 | px, 0 | px), children=[FilledChart(), BarChart()]),
                Row(padding=(0 | px, 0 | px), children=[GradientChart()])
            ]
        ))))


asyncio.run(main())
