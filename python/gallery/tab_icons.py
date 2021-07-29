from p3ui import *
from material_icons import MaterialIcons


class TabIcons(ScrollArea):

    def __init__(self):
        super().__init__(
            content=Flexible(
                width=(100 | percent, 0, 0),
                direction=Direction.Vertical,
                align_items=Alignment.Start,
                justify_content=Justification.Start))
        self.search = InputText(width=(20 | em, 0, 0), label=f'{MaterialIcons.Search}', on_change=self.filter)
        self.content.add(self.search)
        for icon_name in [i for i in MaterialIcons.__dict__.keys() if i[:1] != '_']:
            self.content.add(Text(f'{getattr(MaterialIcons, icon_name)} {icon_name}'))

    def filter(self):
        for child in self.content.children:
            if child is self.search:
                continue
            lbl = child.text.split(' ')[1]
            child.visible = self.search.value.lower() in lbl.lower()
