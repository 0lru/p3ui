from p3ui import Flexible, ComboBox, Direction, Justification, Alignment, px, Button, Style, ScrollArea


class TabFlexible(Flexible):

    def __init__(self):
        super().__init__(
            style=Style(
                direction=Direction.Vertical,
                justify_content=Justification.Center,
                align_items=Alignment.Stretch
            ))

        justifications = [key for key, value in Justification.__members__.items()]

        def set_justification(value):
            self.vertical.style.justify_content = value
            self.horizontal.style.justify_content = value

        self.add(ComboBox(
            label='justify_content (Justification)',
            options=justifications,
            selected_index=1,
            on_change=lambda index: set_justification(Justification.__members__[justifications[index]])
        ))

        alignments = [key for key, value in Alignment.__members__.items()]

        def set_alignment(value):
            self.vertical.style.align_items = value
            self.horizontal.style.align_items = value

        self.add(ComboBox(
            label='align_items (Alignment)',
            options=alignments,
            selected_index=1,
            on_change=lambda index: set_alignment(Alignment.__members__[alignments[index]])
        ))

        self.vertical = Flexible(
            width=(1 | px, 1, 1),
            direction=Direction.Vertical,
            justify_content=Justification.Center,
            align_items=Alignment.Center,
            children=[
                Button(
                    label='B1',
                    width=(48 | px, 1, 1),
                    height=(48 | px, 0, 1)
                ), Button(
                    label='B2',
                    width=(48 | px, 1, 1),
                    height=(48 | px, 0, 1)
                ), Button(
                    label='B3',
                    width=(48 | px, 1, 1),
                    height=(48 | px, 0, 1)
                ), Button(
                    label='B4',
                    width=(48 | px, 1, 1),
                    height=(48 | px, 0, 1)
                ), Button(
                    label='B5',
                    width=(48 | px, 1, 1),
                    height=(48 | px, 0, 1)
                )
            ]
        )

        self.horizontal = Flexible(
            width=(1 | px, 1, 1),
            direction=Direction.Horizontal,
            justify_content=Justification.Center,
            align_items=Alignment.Center,
            children=[
                Button(
                    label='B1',
                    width=(48 | px, 0, 1),
                    height=(48 | px, 1, 1)
                ), Button(
                    label='B2',
                    width=(48 | px, 0, 1),
                    height=(48 | px, 1, 1)
                ), Button(
                    label='B3',
                    width=(48 | px, 0, 1),
                    height=(48 | px, 1, 1)
                ), Button(
                    label='B4',
                    width=(48 | px, 0, 1),
                    height=(48 | px, 1, 1)
                ), Button(
                    label='B5',
                    width=(48 | px, 0, 1),
                    height=(48 | px, 1, 1)
                )
            ]
        )

        self.add(Flexible(
            direction=Direction.Horizontal,
            justify_content=Justification.Center,
            align_items=Alignment.Stretch,
            children=[
                self.vertical,
                self.horizontal
            ]
        ))
