from oogui import *
import numpy as np
from matplotlib.image import imread

icon_char = b'\xee\x8b\x88'.decode('utf-8')


class Widgets(ScrollArea):

    def __init__(self, window, assets):
        super().__init__(
            content=Flexible(
                style=Style(
                    width=(100 | percent, 0, 0),
                    direction=Direction.Vertical,
                    align_items=Alignment.Stretch,
                    justify_content=Justification.Start
                ),
                children=[
                    Text(f'Text'),
                    Text(f'Green Text', style=Style(color='green')),
                    Text(f'Red Text', style=Style(color='#ff0000')),
                    Button(
                        label='Button',
                        on_click=lambda: window.add(Popup(content=Text('button clicked!')))
                    ),
                    Button(
                        label=f"{icon_char} Icon Button",
                        on_click=lambda: print('icon button clicked')
                    ),
                    Text(f'Some Text', label='Label'),
                    CheckBox(
                        label='CheckBox',
                        on_change=lambda value: print(f'checkbox value: {value}')
                    ),
                    InputText(label='InputText', hint="enter sth."),
                    ComboBox(
                        label='ComboBox',
                        options=['aaaa', 'bbbb', 'cccc'],
                        selected_index=1,
                        on_change=lambda index: print(f'combo selected {index}')
                    ),
                    Text(f'Text'),
                    ProgressBar(label='Progress Bar', value=0.4),
                    ProgressBar(value=0.3),
                    ComboBox(
                        options=['aaaa', 'bbbb', 'cccc'],
                        selected_index=1,
                        on_change=lambda index: print(f'combo selected {index}')
                    ),
                    ScrollArea(
                        width=(200 | px, 1, 0),
                        height=(200 | px, 1, 0),
                        content=Image(
                            texture=Texture(
                                (imread(assets.joinpath(
                                    "test.png"
                                ).as_posix()) * 255).astype(np.uint8)
                            ),
                            on_mouse_enter=lambda e: print('mouse entered image'),
                            on_mouse_move=lambda e: print(f'{e.source} {e.x} {e.y}'),
                            on_mouse_leave=lambda e: print('mouse left image'),
                            mouse_tracking_enabled=True
                        )
                    ),
                    SliderU8(
                        min=0, max=100, value=20, label='SliderU8',
                        on_change=lambda value: print(f'SliderU8 value: {value}')
                    ),
                    SliderU16(min=0, max=100, value=20, label='SliderU16'),
                    SliderU32(min=0, max=100, value=20, label='SliderU32'),
                    SliderU64(min=0, max=100, value=20, label='SliderU64'),
                    SliderS8(min=0, max=100, value=20, label='SliderS8'),
                    SliderS16(min=0, max=100, value=20, label='SliderS16'),
                    SliderS32(min=0, max=100, value=20, label='SliderS32'),
                    SliderS64(min=0, max=100, value=20, label='SliderS64'),
                    SliderFloat(min=0, max=100, value=20, label='SliderFloat'),
                    SliderDouble(min=0, max=100, value=20, label='SliderDouble'),
                    SliderFloat(
                        min=0, max=100,
                        value=20,
                        label='SliderFloat (formatted)', format="value=%.3f"
                    ),
                    InputU8(
                        min=0, max=100, value=20, label='InputU8',
                        on_change=lambda value: print(f'InputU8 value: {value}')
                    ),
                    InputU16(min=0, max=100, value=20, label='InputU16', step=1),
                    InputU32(min=0, max=100, value=20, label='InputU32', step=2),
                    InputU64(min=0, max=100, value=20, label='InputU64'),
                    InputS16(min=0, max=100, value=20, label='InputS16'),
                    InputS8(min=0, max=100, value=20, label='InputS8'),
                    InputS32(min=0, max=100, value=20, label='InputS32'),
                    InputS64(min=0, max=100, value=20, label='InputS64'),
                    InputFloat(min=0, max=100, value=20, label='InputFloat'),
                    InputDouble(min=0, max=100, value=20, label='InputDouble'),
                    InputFloat(
                        min=0, max=100, value=20, label='InputFloat (formatted)'
                        , format="value=%.3f"
                    )
                ]
            )
        )
