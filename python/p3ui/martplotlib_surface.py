from p3ui import *
import matplotlib.pyplot as plt
from p3ui.matplotlib.renderer import Renderer


class MatplotlibSurface(Surface):

    def __init__(self, *, dpi, **kwargs):
        super().__init__(**kwargs, on_resize=self._on_resize)
        self.dpi = dpi
        self._figure = plt.figure(dpi=dpi)
        self._ax = self._figure.add_subplot()
        self._renderer = Renderer(1, 1, dpi)

    def _on_resize(self, size):
        width, height = size
        self._figure.set(
            figheight=max(height / self.dpi, 1),
            figwidth=max(width / self.dpi, 1),
            dpi=self.dpi
        )
        self._renderer = Renderer(width, height, self.dpi)
        self._renderer.canvas = Surface.__enter__(self)
        self._figure.draw(self._renderer)
        self._renderer.canvas = None
        Surface.__exit__(self)

    def __enter__(self):
        self._renderer.canvas = Surface.__enter__(self)
        self._ax.clear()
        return self._ax

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._canvas = None
        self._figure.draw(self._renderer)
        Surface.__exit__(self, exc_type, exc_val, exc_tb)
