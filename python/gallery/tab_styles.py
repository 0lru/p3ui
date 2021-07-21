from p3ui import Flexible, Direction, Alignment, Justification, \
    px, em, rem, InputFloat, ComboBox, ScrollArea, ColorEdit, Text, \
    Pixels, Em, Color, Style, Button


class StyleInput(Flexible):
    def __init__(self, label, target, attribute):
        super().__init__(
            width=(100 | em, 1, 0),
            height=(None, 0, 0),
            direction=Direction.Horizontal,
            align_items=Alignment.Center,
            justify_content=Justification.Center,
            padding=(0 | px, 0 | px)
        )
        self.__target = target
        self.__attribute = attribute
        self.add(Text(label + ' ', width=(14 | em, 0, 0)))

    @property
    def value(self):
        return getattr(self.__target, self.__attribute)

    @value.setter
    def value(self, value):
        setattr(self.__target, self.__attribute, value)


class Float1(StyleInput):

    def __init__(self, *args):
        super().__init__(*args)
        self.add(InputFloat(
            on_change=lambda value: setattr(self, 'value', value),
            value=self.value,
            step=0.1))

    def initialize_value(self):
        self.content[0].value = self.value


class Length1(StyleInput):

    def __init__(self, *args):
        super().__init__(*args)
        self.__input = InputFloat(on_change=self.on_change, value=self.value, step=1.)
        self.__combo = ComboBox(
            on_change=self.on_change,
            width=(4 | em, 0, 0),
            selected_index=0 if isinstance(self.value, Pixels) else 1 if isinstance(self.value, Em) else 2,
            options=['px', 'em', 'rem'])
        self.add(self.__input)
        self.add(self.__combo)

    def initialize_value(self):
        self.__combo.selected_index = 0 if isinstance(self.value, Pixels) else 1 if isinstance(self.value, Em) else 2

    def on_change(self, _):
        self.value = self.__input.value | [px, em, rem][self.__combo.selected_index]


class Length2(Flexible):

    def __init__(self, label, target, attribute):
        super().__init__(
            width=(100 | em, 1, 0),
            height=(None, 0, 0),
            direction=Direction.Horizontal,
            align_items=Alignment.Center,
            justify_content=Justification.SpaceBetween,
            padding=(0 | px, 0 | px)
        )
        self.__target = target
        self.__attribute = attribute
        self.__text = Text(label + ' ', width=(14 | em, 0, 0))
        self.__input1 = InputFloat(on_change=self.on_change, value=8., step=1.)
        self.__combo1 = ComboBox(on_change=self.on_change, width=(4 | em, 0, 0), selected_index=0,
                                 options=['px', 'em', 'rem'])
        self.__input2 = InputFloat(on_change=self.on_change, value=8., step=1.)
        self.__combo2 = ComboBox(on_change=self.on_change, width=(4 | em, 0, 0), selected_index=0,
                                 options=['px', 'em', 'rem'])

        self.add(self.__text)
        self.add(self.__input1)
        self.add(self.__combo1)
        self.add(self.__input2)
        self.add(self.__combo2)

        self.initialize_value()

    def initialize_value(self):
        initial = getattr(self.__target, self.__attribute)
        self.__input1.value = initial[0].value
        if isinstance(initial[0], Pixels):
            self.__combo1.selected_index = 0
        elif isinstance(initial[0], Em):
            self.__combo1.selected_index = 1
        else:
            self.__combo1.selected_index = 2

        self.__input2.value = initial[1].value
        if isinstance(initial[1], Pixels):
            self.__combo2.selected_index = 0
        elif isinstance(initial[1], Em):
            self.__combo2.selected_index = 1
        else:
            self.__combo2.selected_index = 2

    def on_change(self, _):
        l2 = (
            self.__input1.value | [px, em, rem][self.__combo1.selected_index],
            self.__input2.value | [px, em, rem][self.__combo2.selected_index]
        )
        setattr(self.__target, self.__attribute, l2)


class Color4(Flexible):

    def __init__(self, label, target, attribute):
        super().__init__(
            width=(12 | em, 1, 0),
            height=(None, 0, 0),
            direction=Direction.Horizontal,
            align_items=Alignment.Center,
            justify_content=Justification.SpaceBetween,
            padding=(0 | px, 0 | px)
        )
        self.__target = target
        self.__attribute = attribute

        self.__text = Text(label + ' ', width=(14 | em, 0, 0))
        self.__edit = ColorEdit(
            width=(30 | em, 1, 0),
            height=(None, 0, 0),
            on_change=self.on_change
        )
        self.add(self.__text)
        self.add(self.__edit)
        self.__edit.value = getattr(target, attribute)

    def on_change(self, _):
        setattr(self.__target, self.__attribute, self.__edit.value)

    def initialize_value(self):
        self.__edit.value = getattr(self.__target, self.__attribute)


class TabStyles(ScrollArea):

    def initialize_values(self):
        for child in self.content.children:
            if hasattr(child, 'initialize_value'):
                child.initialize_value()

    def make_light(self):
        self.ui.theme.make_light()
        self.initialize_values()

    def make_dark(self):
        self.ui.theme.make_dark()
        self.initialize_values()

    def make_classic(self):
        self.ui.theme.make_classic()
        self.initialize_values()

    def __init__(self, ui):
        super().__init__(content=Flexible(
            width=(45 | em, 0, 0),
            direction=Direction.Vertical,
            align_items=Alignment.Stretch,
            justify_content=Justification.Start))
        self.ui = ui
        self.content.add(Flexible(
            style=Style(
                direction=Direction.Horizontal,
                padding=(0 | px, 0.5 | em)
            ),
            children=[
                Button(
                    label='make light',
                    on_click=self.make_light
                ),
                Button(
                    label='make dark',
                    on_click=self.make_dark
                ),
                Button(
                    label='make classic',
                    on_click=self.make_classic
                )
            ]
        ))
        for name in [x for x in dir(ui.theme) if not x.startswith('_')]:
            value = getattr(ui.theme, name)
            label = ' '.join([c.title() for c in (name.split('_'))])
            if value is None:
                print(f'[info] {name} is not set')
            elif isinstance(value, Color):
                self.content.add(Color4(label, ui.theme, name))
        self.content.add(Length1('Tab Rounding', ui.theme, 'tab_rounding'))
        self.content.add(Length1('Window Rounding', ui.theme, 'window_rounding'))
        self.content.add(Length1('Window Border Size', ui.theme, 'window_border_size'))
        self.content.add(Length1('Child Rounding', ui.theme, 'child_rounding'))
        self.content.add(Length1('Child Border Size', ui.theme, 'child_border_size'))
        self.content.add(Length1('Frame Rounding', ui.theme, 'frame_rounding'))
        self.content.add(Length1('Frame Border Size', ui.theme, 'frame_border_size'))
        self.content.add(Length2('Frame Padding', ui.theme, 'frame_padding'))
        self.content.add(Length1('Popup Rounding', ui.theme, 'popup_rounding'))
        self.content.add(Length1('Popup Border Size', ui.theme, 'popup_border_size'))
        self.content.add(Length1('Indent Spacing', ui.theme, 'indent_spacing'))
        self.content.add(Length1('Scrollbar Size', ui.theme, 'scrollbar_size'))
        self.content.add(Length1('Scrollbar Rounding', ui.theme, 'scrollbar_rounding'))
        self.content.add(Length1('Grab Min Size', ui.theme, 'grab_min_size'))
        self.content.add(Length1('Grab Rounding', ui.theme, 'grab_rounding'))
        self.content.add(Length2('Item Spacing', ui.theme, 'item_spacing'))
        self.content.add(Length2('Inner Item Spacing', ui.theme, 'item_inner_spacing'))
        self.content.add(Length2('Window Padding', ui.theme, 'window_padding'))
        self.content.add(Length2('Cell Padding', ui.theme, 'cell_padding'))
