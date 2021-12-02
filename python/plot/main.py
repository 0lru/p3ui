import matplotlib.pyplot as plt
from p3ui import *
from p3.matplotlib.matplotlib_figure import MatplotlibFigure
import asyncio
import numpy as np


def sin_generator():
    shift = 0.0
    while True:
        shift += 0.1
        x = np.arange(0 + shift, 10.0 + shift, 0.1)
        yield x, np.sin(x)


async def update(figure):
    generator = sin_generator()
    x, y = next(generator)
    print(len(x))
    while True:
        x, y = next(generator)
        with figure as ax:
            ax.clear()
            ax.plot(x, y)
        await asyncio.sleep(0.01)

async def main():
    window = Window(title='Canvas Demo')
    window.position = (50, 50)
    window.size = (800, 640)

    figure = MatplotlibFigure()

    asyncio.create_task(update(figure))

    with figure as plt:
        x = np.arange(0., 10., 0.2)
        plt.plot(x, np.sin(x))

    await window.serve(UserInterface(content=figure))


asyncio.run(main())
