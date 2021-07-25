from p3ui import *


class VerticalScrollArea(ScrollArea):

    def __init__(self, *args, **kwargs):
        super().__init__(
            content=Flexible(
                width=(100 | percent, 0, 0),
                height=(auto, 0, 0),
                direction=Direction.Vertical,
                align_items=Alignment.Stretch,
                justify_content=Justification.Center))
