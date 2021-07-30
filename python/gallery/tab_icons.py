from p3ui import *
from material_icons import MaterialIcons


class TabIcons(Layout):

    def __init__(self):
        super().__init__(
            content=Layout(
                width=(100 | percent, 0, 0),
                direction=Direction.Vertical,
                align_items=Alignment.Start,
                justify_content=Justification.Start))

        self.search = InputText(
            label=f'{MaterialIcons.Search}',
            width=(20 | em, 0, 0),
            height=(None, 0, 0),
            on_change=self.filter)
        self.add(self.search)

        self.icons_list = [
            Text(f'{getattr(MaterialIcons, icon_name)} {icon_name}')
            for icon_name in TabIcons.icon_names()
        ]
        self.add(ScrollArea(content=Layout(children=self.icons_list)))

    @staticmethod
    def icon_names():
        for icon_name in [i for i in MaterialIcons.__dict__.keys() if i[:1] != '_']:
            yield icon_name

    def filter(self):
        for icon_text in self.icons_list:
            if icon_text is self.search:
                continue
            lbl = icon_text.value.split(' ')[1]
            icon_text.visible = self.search.value.lower() in lbl.lower()
