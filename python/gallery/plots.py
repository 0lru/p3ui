from p3ui import Flexible, ScrollArea, Plot, Style, percent, auto, px, \
    percent, Direction, Alignment, Justification, MarkerStyle
import numpy as np


class Plots(ScrollArea):

    def __init__(self):
        #
        # create scroll area with scrolled vertical, flexible
        super().__init__(
            content=Flexible(
                width=(100 | percent, 0, 0),
                height=(auto, 0, 0),
                direction=Direction.Vertical,
                align_items=Alignment.Stretch,
                justify_content=Justification.Center))

        #
        # sinus generator
        self.sin_signal = Plots.create_sin_signal()

        #
        # define sinus plot with height of 300 pixels, not allowed to shrink (vertically)
        plot = Plot(
            label='Sinus',
            y_limits=(-1.1, 1.1),
            style=Style(
                width=(250 | px, 1, 1),
                height=(300 | px, 1, 0)
            ))
        self.line_series = Plot.LineSeriesDouble("sin")
        plot.add(self.line_series)
        plot.add(Plot.HorizontalLinesFloat("lines", data=[0.5]))
        self.content.add(plot)

        #
        # bar plot
        plot = Plot(
            label='Bars',
            style=Style(
                width=(250 | px, 1, 1),
                height=(300 | px, 1, 0)),
            x_ticks=[0, 1, 2, 3, 4, 5, 6, 7],
            x_tick_labels=['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight'],
            x_limits=(-1, 8),
            y_limits=(0, 35))
        plot.add(Plot.BarSeriesFloat("s1", width=0.2, shift=-0.1, values=[1, 5, 3, 5, 30, 5, 2, 1]))
        plot.add(Plot.BarSeriesFloat("s2", width=0.2, shift=0.1, values=[2, 4, 8, 3, 15, 13, 11, 4]))
        self.content.add(plot)

        #
        # scatter plot
        plot = Plot(
            label='Scatter',
            y_limits=(0, 1.1),
            style=Style(
                width=(250 | px, 1, 1),
                height=(300 | px, 1, 0)))
        x_scatter = np.arange(0, 10, 0.1)
        y_scatter = np.random.rand(x_scatter.shape[0])
        self.scatter_series = Plot.ScatterSeriesFloat(
            "scatter",
            x=x_scatter, y=y_scatter,
            marker_style=MarkerStyle.Circle)
        plot.add(self.scatter_series)
        self.content.add(plot)

        #
        # stems
        #
        # define sinus plot with height of 300 pixels, not allowed to shrink (vertically)
        plot = Plot(
            label='Stems',
            y_limits=(-0.1, 1.1),
            stlye=Style(
                width=(250 | px, 1, 1), height=(300 | px, 1, 0)))
        self.stem_series = Plot.StemSeriesFloat("sin")
        x = np.arange(0, 100)
        plot.add(Plot.StemSeriesFloat("stems", x=x, y=np.random.rand(x.shape[0])))
        self.content.add(plot)

    def update(self):
        self.line_series.x, self.line_series.y = next(self.sin_signal)
        self.scatter_series.y = np.random.rand(self.scatter_series.y.shape[0])

    @staticmethod
    def create_sin_signal():
        shift = 0.0
        while True:
            shift += 0.01
            x = np.arange(10000 + shift, 10010.0 + shift, 0.001)
            yield x, np.sin(x)
