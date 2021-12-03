from p3ui import *
import asyncio
import numpy as np
from bar_plot import BarPlot
from filled_plot import FilledPlot


async def main():
    window = Window(title='matplotlib')
    window.position = (50, 50)
    window.size = (800, 640)

    dpi = window.monitor.dpi

    await window.serve(UserInterface(
        content=Column(
            children=[
                Button(label='button 1'),
                Row(children=[BarPlot(dpi), FilledPlot(dpi)]),
                Button(label='button 2'),
                Row(children=[FilledPlot(dpi), BarPlot(dpi)])
            ]
        )))


asyncio.run(main())
