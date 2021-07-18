import p3ui as p3


class ViewOptions(p3.ScrollArea):

    def __init__(self, window):
        self.window = window

        super().__init__()

        self.monitors = window.monitors()

        primary_monitor = window.primary_monitor()
        for monitor in self.monitors:
            print(f'EQUAL{monitor == primary_monitor}')

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

        self.content = p3.Flexible(
            style=p3.Style(
                width=(100 | p3.percent, 1, 1),
                height=(None, 0, 0),
                direction=p3.Direction.Vertical,
                align_items=p3.Alignment.Stretch,
                justify_content=p3.Justification.Start
            ),
            children=[
                p3.Collapsible(
                    label='Video Mode',
                    collapsed=False,
                    style=p3.Style(
                    ),
                    content=p3.Flexible(
                        style=p3.Style(
                            direction=p3.Direction.Vertical,
                            justify_content=p3.Justification.Start,
                            align_items=p3.Alignment.Stretch,
                            padding=(1 | p3.em, 0 | p3.em),
                        ),
                        children=[
                            self.monitor_combo_box,
                            self.mode_combo_box,
                            p3.Flexible(
                                style=p3.Style(
                                    direction=p3.Direction.Horizontal,
                                    padding=(0 | p3.em, 0 | p3.em),
                                    align_items=p3.Alignment.End
                                ),
                                children=[
                                    p3.Button(
                                        label='apply',
                                        style=p3.Style(
                                            width=(100 | p3.px, 0, 0),
                                        ),
                                        on_click=self.set_video_mode
                                    ),
                                    p3.Button(
                                        label='reset',
                                        style=p3.Style(
                                            width=(100 | p3.px, 0, 0),
                                        ),
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
                    content=p3.Flexible(
                        style=p3.Style(
                            justify_content=p3.Justification.Start,
                            align_items=p3.Alignment.Stretch,
                            padding=(1 | p3.em, 0 | p3.em)
                        ),
                        children=[
                            self.vsync_checkbox
                        ]
                    )
                )
            ]
        )

    def update_video_modes(self):
        self.mode_combo_box.options = [f'{mode.width}x{mode.height} {mode.hz}Hz' for mode in self.selected_monitor.modes]
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


