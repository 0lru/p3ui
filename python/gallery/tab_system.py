import time
import p3ui as p3
import psutil
import asyncio
import numpy as np
from datetime import datetime


class ProcessInfo(p3.Layout):

    @staticmethod
    def utc_seconds(dt):
        return (dt - datetime(1, 1, 1)).total_seconds()

    @classmethod
    def utc_now_seconds(cls):
        return cls.utc_seconds(datetime.utcnow())

    async def update(self):
        while True:
            self.__frames_per_second.value = f'{self.window.frames_per_second:.2f}'
            self.__seconds_till_idle.value = f'{self.window.time_till_enter_idle_mode:.2f}'
            self._memory_series.x = np.roll(self._memory_series.x, -1)
            self._memory_series.y = np.roll(self._memory_series.y, -1)
            self._memory_series.x[-1] = time.time()
            self._memory_series.y[-1] = self.memory_usage
            self._cpu_series.x = np.roll(self._cpu_series.x, -1)
            self._cpu_series.y = np.roll(self._cpu_series.y, -1)
            self._cpu_series.x[-1] = time.time()
            self._cpu_series.y[-1] = self.cpu_usage
            await asyncio.sleep(1)

    def __init__(self, window):
        super().__init__(
            direction=p3.Direction.Vertical,
            justify_content=p3.Justification.Start,
            align_items=p3.Alignment.Stretch,
            padding=(1 | p3.em, 0 | p3.em)
        )
        self.window = window
        self._process = psutil.Process()

        self.__frames_per_second = p3.Text('-', label='Frames Per Second')
        self.__seconds_till_idle = p3.Text('-', label='Seconds Till Idle')
        self.add(self.__frames_per_second)
        self.add(self.__seconds_till_idle)

        plot = p3.Plot(
            label='Process Memory Usage',
            x_label='Time (UTC)',
            y_label='GB',
            x_type=p3.Plot.Axis.UniversalTime,
            width=(250 | p3.px, 1, 1),
            height=(18 | p3.em, 1, 0))
        plot.x_axis.type = p3.Plot.Axis.UniversalTime
        self._memory_series = p3.Plot.LineSeriesDouble(self._process.name())
        self._memory_series.x = np.arange(-100.0, .0, 1.0) + time.time()
        self._memory_series.y = np.zeros(100)
        plot.add(self._memory_series)
        self.add(plot)

        plot = p3.Plot(
            label='CPU Usage',
            x_label='Time (UTC)',
            y_label='%',
            y_limits=(-0.1, 100.1),
            x_type=p3.Plot.Axis.UniversalTime,
            width=(250 | p3.px, 1, 1),
            height=(18 | p3.em, 1, 0))
        self._cpu_series = p3.Plot.LineSeriesDouble(self._process.name())
        self._cpu_series.x = np.arange(-100.0, .0, 1.0) + time.time()
        self._cpu_series.y = np.zeros(100)
        plot.add(self._cpu_series)
        self.add(plot)

    @property
    def memory_usage(self):
        return self._process.memory_info().rss / 1024 ** 3

    @property
    def cpu_usage(self):
        return self._process.cpu_percent() / psutil.cpu_count()


class TabSystem(p3.ScrollArea):

    async def update(self):
        await self.__process_info.update()

    def __init__(self, window):
        self.window = window

        window.idle_timeout = 60.0
        window.idle_frame_time = 1.0
        super().__init__()

        self.monitors = window.monitors()

        self.monitor_combo_box = p3.ComboBox(
            label='Monitor',
            selected_index=0,
            options=[monitor.name for monitor in self.monitors]
        )

        self.mode_combo_box = p3.ComboBox(
            label='Video Mode'
        )
        self.update_video_modes()

        self.vsync_checkbox = p3.CheckBox(
            label='VSync Enabled',
            value=window.vsync,
            on_change=self.set_vsync_enabled
        )

        self.__process_info = ProcessInfo(window)

        self.content = p3.Layout(
            width=(100 | p3.percent, 1, 1),
            height=(None, 0, 0),
            direction=p3.Direction.Vertical,
            align_items=p3.Alignment.Stretch,
            justify_content=p3.Justification.Start,
            children=[
                p3.Collapsible(
                    label='Video Mode',
                    collapsed=False,
                    content=p3.Layout(
                        direction=p3.Direction.Vertical,
                        justify_content=p3.Justification.Start,
                        align_items=p3.Alignment.Stretch,
                        padding=(1 | p3.em, 0 | p3.em),
                        children=[
                            self.monitor_combo_box,
                            self.mode_combo_box,
                            p3.Layout(
                                direction=p3.Direction.Horizontal,
                                padding=(0 | p3.em, 0 | p3.em),
                                align_items=p3.Alignment.End,
                                children=[
                                    p3.Button(
                                        label='apply',
                                        width=(100 | p3.px, 0, 0),
                                        on_click=self.set_video_mode
                                    ),
                                    p3.Button(
                                        label='reset',
                                        width=(100 | p3.px, 0, 0),
                                        on_click=self.reset_video_mode
                                    )
                                ]
                            ),
                        ]
                    )
                ),
                p3.Collapsible(
                    label='Settings',
                    collapsed=False,
                    content=p3.Layout(
                        justify_content=p3.Justification.Start,
                        align_items=p3.Alignment.Stretch,
                        padding=(1 | p3.em, 0 | p3.em),
                        children=[
                            p3.InputDouble(
                                label='Idle Timeout (seconds)',
                                value=window.idle_timeout,
                                step=1.0,
                                on_change=lambda value: setattr(window, 'idle_timeout', value)
                            ),
                            p3.InputDouble(
                                label='Idle Frame Time (seconds)',
                                value=window.idle_frame_time,
                                step=0.1,
                                on_change=lambda value: setattr(window, 'idle_frame_time', value)
                            ),
                            self.vsync_checkbox,
                        ]
                    )
                ),
                p3.Collapsible(
                    label='System Status',
                    collapsed=False,
                    content=self.__process_info
                )
            ]
        )

    def update_video_modes(self):
        self.mode_combo_box.options = [f'{mode.width}x{mode.height} {mode.hz}Hz' for mode in
                                       self.selected_monitor.modes]
        self.mode_combo_box.selected_index = len(self.mode_combo_box.options) - 1

    @property
    def selected_monitor(self):
        return self.monitors[self.monitor_combo_box.selected_index]

    @property
    def selected_video_mode(self):
        return self.selected_monitor.modes[self.mode_combo_box.selected_index]

    def set_video_mode(self):
        self.window.video_mode = self.selected_video_mode

    def reset_video_mode(self):
        self.window.video_mode = None

    def set_vsync_enabled(self, vsync):
        self.window.vsync = vsync
