from p3ui import *
from p3.matplotlib.matplotlib_surface import MatplotlibSurface


class BarPlot(MatplotlibSurface):
    # https://matplotlib.org/stable/gallery/lines_bars_and_markers/bar_stacked.html#sphx-glr-gallery-lines-bars-and-markers-bar-stacked-py

    def __init__(self, dpi):
        super().__init__(dpi=dpi, width=(auto, 1, 1), height=(auto, 1, 1))
        self._update()

    def _update(self):
        with self as ax:
            labels = ['G1', 'G2', 'G3', 'G4', 'G5']
            men_means = [20, 35, 30, 35, 27]
            women_means = [25, 32, 34, 20, 25]
            men_std = [2, 3, 4, 1, 2]
            women_std = [3, 5, 2, 3, 3]
            width = 0.35  # the width of the bars: can also be len(x) sequence

            ax.bar(labels, men_means, width, yerr=men_std, label='Men')
            ax.bar(labels, women_means, width, yerr=women_std, bottom=men_means,
                   label='Women')

            ax.set_ylabel('Scores')
            ax.set_title('Scores by group and gender')
            ax.legend()

    async def update(self):
        self._update()
