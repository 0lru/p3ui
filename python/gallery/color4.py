from oogui import Pixels, Em, Flexible, em, px, rem, auto,\
    Direction, Alignment, Justification, ComboBox, InputFloat, \
    ColorEdit, Text


class Color4(Flexible):

    def __init__(self, label, target, attribute):
        super().__init__(
            width=(12 | em, 1, 0),
            height=(auto, 0, 0),
            direction=Direction.Horizontal,
            align_items=Alignment.Center,
            justify_content=Justification.SpaceBetween
        )
        self.__target = target
        self.__attribute = attribute

        self.__text = Text(label + ' ', width=(14 | em, 0, 0))
        self.__edit = ColorEdit(
            width=(30 | em, 1, 0),
            height=(auto, 0, 0),
            on_change=self.on_change
        )
        self.add(self.__text)
        self.add(self.__edit)
        self.__edit.value = getattr(target, attribute)

    def on_change(self, _):
        setattr(self.__target, self.__attribute, self.__edit.value)
