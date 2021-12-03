from p3ui import *
import matplotlib.pyplot as plt
import numpy as np


def gradient_image(ax, extent, direction=0.3, cmap_range=(0, 1), **kwargs):
    phi = direction * np.pi / 2
    v = np.array([np.cos(phi), np.sin(phi)])
    X = np.array([[v @ [1, 0], v @ [1, 1]],
                  [v @ [0, 0], v @ [0, 1]]])
    a, b = cmap_range
    X = a + (b - a) / X.max() * X
    im = ax.imshow(X, extent=extent, interpolation='bicubic',
                   vmin=0, vmax=1, **kwargs)
    return im


def gradient_bar(ax, x, y, width=0.5, bottom=0):
    for left, top in zip(x, y):
        right = left + width
        gradient_image(ax, extent=(left, right, bottom, top),
                       cmap=plt.cm.Blues_r, cmap_range=(0, 0.8))


class GradientChart(MatplotlibSurface):
    # https://matplotlib.org/stable/gallery/lines_bars_and_markers/bar_stacked.html#sphx-glr-gallery-lines-bars-and-markers-bar-stacked-py

    def __init__(self, dpi):
        super().__init__(dpi=dpi, width=(auto, 1, 1), height=(auto, 1, 1))
        self._update()

    def _update(self):
        with self as ax:
            np.random.seed(19680801)
            ax.set(xlim=(0, 10), ylim=(0, 1), autoscale_on=False)
            gradient_image(ax, direction=1, extent=(0, 1, 0, 1), transform=ax.transAxes,
                           cmap=plt.cm.RdYlGn, cmap_range=(0.2, 0.8), alpha=0.5)
            N = 10
            x = np.arange(N) + 0.15
            y = np.random.rand(N)
            gradient_bar(ax, x, y, width=0.7)
            ax.set_aspect('auto')

    async def update(self):
        self._update()
