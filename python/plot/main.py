from p3ui import *
import asyncio
from bar_chart import BarChart
from filled_chart import FilledChart
from gradient_chart import GradientChart


async def main():
    window = Window(title='matplotlib')
    window.position = (50, 50)
    window.size = (800, 800)

    dpi = window.monitor.dpi

    await window.serve(UserInterface(
        content=Column(
            children=[
                Row(children=[BarChart(dpi), FilledChart(dpi)]),
                Row(children=[FilledChart(dpi), BarChart(dpi)]),
                Row(children=[GradientChart(dpi)])
            ]
        )))


asyncio.run(main())
