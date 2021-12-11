from p3ui import *
import matplotlib.pyplot as plt
from .mpl import Renderer
from .mpl import dpi as default_dpi


class MatplotlibSurface(Surface):

    def __init__(self, *, dpi=default_dpi, **kwargs):
        super().__init__(**kwargs, on_resize=self._on_resize)
        self.dpi = dpi
        self._figure = plt.figure(dpi=dpi)
        self._renderer = Renderer(1, 1, dpi)

    def _on_resize(self, size):
        width, height = size
        self._figure.set(
            figwidth=max(width / self.dpi, 1),
            figheight=max(height / self.dpi, 1),
            dpi=self.dpi
        )
        self._renderer = Renderer(width, height, self.dpi)
        self._renderer.canvas = Surface.__enter__(self)
        self._figure.draw(self._renderer)
        self._renderer.canvas = None
        Surface.__exit__(self)

    def __enter__(self):
        self._renderer.canvas = Surface.__enter__(self)
        return self._figure

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._canvas = None
        self._figure.draw(self._renderer)
        Surface.__exit__(self, exc_type, exc_val, exc_tb)
