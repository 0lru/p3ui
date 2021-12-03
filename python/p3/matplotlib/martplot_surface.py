from p3ui import *
import matplotlib
import matplotlib.pyplot as plt
from p3.matplotlib.backend import Renderer
import skia

matplotlib.use("module://p3.matplotlib.backend")


class MatplotSurface(Surface):

    def __init__(self, *, dpi=None, **kwargs):
        super().__init__(**kwargs)
        self._figure = plt.figure()
        self._ax = self._figure.add_subplot()
        self._renderer = Renderer(dpi if dpi else self._figure.dpi)

    def __enter__(self):
        self._renderer.canvas = Surface.__enter__(self)
        self._ax.clear()
        return self._ax

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._canvas = None
        self._figure.draw(self._renderer)
        Surface.__exit__(self, exc_type, exc_val, exc_tb)
