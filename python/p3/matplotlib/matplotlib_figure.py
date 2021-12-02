from p3ui import *
import matplotlib
import matplotlib.pyplot as plt
from p3.matplotlib.backend import Renderer
import skia
matplotlib.use("module://p3.matplotlib.backend")


class MatplotlibFigure(Surface):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._figure = plt.figure()
        self._ax = self._figure.add_axes([0.05, 0.05, 0.9, 0.9])
        self._renderer = Renderer(self._figure.dpi)

    def __enter__(self):
        self._renderer.canvas = Surface.__enter__(self)
        self._renderer.canvas.clear(skia.ColorWHITE)
        return self._ax

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._canvas = None
        self._figure.draw(self._renderer)
        Surface.__exit__(self, exc_type, exc_val, exc_tb)
