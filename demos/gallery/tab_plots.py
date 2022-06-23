from p3ui import *
import numpy as np
import asyncio
import time

from vertical_scroll_area import VerticalScrollArea


class ColoredPlot(Plot):

    def __init__(self, label, **kwargs):
        super().__init__(
            label='Sinus',
            y_limits=(-0.1, 1.1),
            width=(250 | px, 1, 1),
            height=(300 | px, 1, 0),
            **kwargs
        )
        self.add(Plot.LineSeriesDouble(
            "red with yellow marker",
            x=np.arange(0., 20., 1.),
            y=np.random.random(20),
            line_color='red',
            marker_style=MarkerStyle.Circle,
            marker_line_color='black',
            marker_fill_color='yellow'
        ))
        self.add(Plot.LineSeriesDouble(
            "blue with yellow marker",
            x=np.arange(0., 20., 1.),
            y=np.random.random(20),
            line_color='blue',
            marker_style=MarkerStyle.Circle,
            marker_line_color='black',
            marker_fill_color='yellow'
        ))
        self.add(Plot.LineSeriesDouble(
            "white with big white marker",
            x=np.arange(0., 20., 1.),
            y=np.random.random(20),
            opacity=0.5,
            line_color='white',
            marker_style=MarkerStyle.Circle,
            marker_line_color='black',
            marker_fill_color='white',
            marker_size=4 | px
        ))


class TabPlots(VerticalScrollArea):

    def __init__(self):
        super().__init__()

        #
        # sinus generator
        self.sin_signal = TabPlots.create_sin_signal()

        #
        # define sinus plot with height of 300 pixels, not allowed to shrink (vertically)
        self.line_plot = Plot(
            label='Sinus',
            y_limits=(-1.1, 1.1),
            width=(250 | px, 1, 1),
            height=(300 | px, 1, 0)
        )
        self.line_series = Plot.LineSeriesDouble("sin", colormap=Plot.Colormap.Spectral)
        self.line_plot.add(self.line_series)
        self.line_plot.add(Plot.HorizontalLinesFloat("lines", values=[0.5]))
        self.line_series.add(Plot.Annotation('hline', y=0.5, clamped=True))
        self.content.add(self.line_plot)

        #
        # bar plot
        plot = Plot(
            label='Bars',
            width=(250 | px, 1, 1),
            height=(300 | px, 1, 0),
            x_ticks=[0, 1, 2, 3, 4, 5, 6, 7],
            x_tick_labels=['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight'],
            x_limits=(-1, 8))
        bar_series1 = Plot.BarSeriesFloat("s1", width=0.2, shift=-0.1, values=[1, 5, 3, 5, 30, 5, 2, 1])
        bar_series2 = Plot.BarSeriesFloat("s2", width=0.2, shift=0.1, values=[2, 4, 8, 3, 15, 13, 11, 4])
        plot.add(bar_series1)
        plot.add(bar_series2)
        plot.add(Plot.Annotation('plot-annotation', x=0, y=10))
        test = Plot.Annotation('plot-annotation, colored', x=0, y=15, fill_color='red')
        plot.add(test)
        plot.remove(test)
        bar_series1.add(Plot.Annotation('series-annotation', x=2, y=12, clamped=True))
        bar_series2.add(Plot.Annotation('series-annotation', x=2, y=16))
        bar_series1.add(Plot.Annotation('series-annotation', x=3, y=15,
                                        line_color='red',
                                        fill_color='#00000000'))
        self.content.add(plot)

        #
        # scatter plot
        plot = Plot(label='Scatter', y_limits=(0, 1.1), width=(250 | px, 1, 1), height=(300 | px, 1, 0))
        x_scatter = np.arange(0, 10, 0.1)
        y_scatter = np.random.rand(x_scatter.shape[0])
        self.scatter_series = Plot.ScatterSeriesFloat("scatter", x=x_scatter, y=y_scatter,
                                                      marker_style=MarkerStyle.Circle)
        plot.add(self.scatter_series)
        self.content.add(plot)

        #
        # stems
        #
        plot = Plot(label='Stems', y_limits=(-0.1, 1.1), width=(250 | px, 1, 1), height=(300 | px, 1, 0))
        self.stem_series = Plot.StemSeriesDouble("sin")
        x = np.arange(0, 100)
        plot.add(Plot.StemSeriesFloat("stems", x=x, y=np.random.rand(x.shape[0])))
        self.content.add(plot)

        self.content.add(ColoredPlot(label='Colored Plot'))

    @staticmethod
    def create_sin_signal():
        start = time.time()
        while True:
            shift = time.time() - start
            x = np.arange(0 + shift, 10.0 + shift, 0.001)
            yield x, np.sin(x)

    async def update(self):
        try:
            x, y = next(self.sin_signal)
            self.line_plot.x_axis.limits = (x.min(), x.max())
            while True:
                _, y = next(self.sin_signal)
                self.line_plot.x_axis.limits = (x.min(), x.max())
                self.line_series.x = x
                self.line_series.y = y
                self.scatter_series.y = np.random.rand(self.scatter_series.y.shape[0])
                # await asyncio.sleep(1.0/60.0)
                await asyncio.sleep(0)
        except asyncio.CancelledError:
            pass
