from p3ui import *
from cycler import cycler
import numpy as np
import matplotlib as mpl

# Define a list of markevery cases and color cases to plot
cases = [None,
         8,
         (30, 8),
         [16, 24, 30],
         [0, -1],
         slice(100, 200, 3),
         0.1,
         0.3,
         1.5,
         (0.0, 0.1),
         (0.45, 0.1)]

colors = ['#1f77b4',
          '#ff7f0e',
          '#2ca02c',
          '#d62728',
          '#9467bd',
          '#8c564b',
          '#e377c2',
          '#7f7f7f',
          '#bcbd22',
          '#17becf',
          '#1a55FF']

# Configure rcParams axes.prop_cycle to simultaneously cycle cases and colors.
mpl.rcParams['axes.prop_cycle'] = cycler(markevery=cases, color=colors)

# Create data points and offsets
x = np.linspace(0, 2 * np.pi)
offsets = np.linspace(0, 2 * np.pi, 11, endpoint=False)
yy = np.transpose([np.sin(x + phi) for phi in offsets])


class MarkEvery(MatplotlibSurface):

    def __init__(self, **kwargs):
        width = kwargs.pop('width', (auto, 1, 1))
        height = kwargs.pop('height', (auto, 1, 1))
        super().__init__(width=width, height=height, **kwargs)
        self._update()

    def _update(self):
        with self as figure:
            ax = figure.add_axes([0.1, 0.1, 0.6, 0.75])

            for i in range(len(cases)):
                ax.plot(yy[:, i], marker='o', label=str(cases[i]))
                ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
