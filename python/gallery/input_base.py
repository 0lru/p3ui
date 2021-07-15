from oogui import Flexible, Direction, Alignment, Justification, auto, em, Text

flexible = Flexible()
flexible.add()


class InputBase(Flexible):
    def __init__(self, **kwargs):
        super().__init__(
            width=(100 | em, 1, 0),
            height=(auto, 0, 0),
            direction=Direction.Horizontal,
            align_items=Alignment.Center,
            justify_content=Justification.Center
        )
        label = kwargs.pop('label', None)
        if label is not None:
            self.add(Text(label + ' ', width=(14 | em, 0, 0)))
