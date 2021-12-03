from p3ui import *
from p3ui.matplotlib.surface import Surface
import numpy as np

class FilledChart(Surface):

    def __init__(self, dpi):
        super().__init__(dpi=dpi, width=(auto, 1, 1), height=(auto, 1, 1))
        self._update()

    def _update(self):
        with self as ax:
            x = np.arange(0.0, 2, 0.01)
            y1 = np.sin(2 * np.pi * x)
            ax.fill_between(x, y1)
            ax.set_title('fill between y1 and 0')

    async def update(self):
        self._update()
