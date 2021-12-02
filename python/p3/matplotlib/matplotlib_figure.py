from p3ui import *
import matplotlib
import matplotlib.pyplot as plt

matplotlib.use("module://p3.matplotlib.backend")


class MatplotlibFigure(Surface):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._native_canvas = plt.figure()

    def __enter__(self):
        return plt

    def __exit__(self, exc_type, exc_val, exc_tb):
        pass
