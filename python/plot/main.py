from p3ui import *
from p3.matplotlib.matplotlib_figure import MatplotlibFigure
import asyncio
import numpy as np


async def main():
    window = Window(title='Canvas Demo')
    window.position = (50, 50)
    window.size = (800, 640)

    figure = MatplotlibFigure()

    with figure as plt:
        x = np.arange(0., 10., 0.2)
        plt.plot(x, np.sin(x))

    await window.serve(UserInterface(content=figure))


asyncio.run(main())
