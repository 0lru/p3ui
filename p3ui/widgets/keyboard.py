import asyncio
from ..native import ChildWindow, em, Column, Button, auto, Alignment, Row, InputText


class Keyboard(ChildWindow):

    def __init__(self, user_interface, item_size=2.5, spacing=0.25, unit=em):
        self.user_interface = user_interface
        row1 = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0']
        row2 = ['q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '0']
        row3 = ['a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '.', '0']
        row4 = ['y', 'x', 'c', 'v', 'b', 'n', 'm', ',', 'l']
        s = item_size | unit
        space = spacing | unit
        s2 = (item_size * 2 + spacing) | unit
        self.target = None
        self._task = None
        self._input = []

        def cbutton(**kwargs):
            l = kwargs['label']

            def on_click():
                self._input.append(ord(l[0]))
                #                if self._task:
                #                    self._task.cancel()
                #                    self._task = None
                self.target.focus()

            #                self._task = asyncio.get_event_loop().create_task(self.push_input())

            return Button(**kwargs, on_click=on_click)

        super().__init__(
            visible=False,
            on_close=lambda: setattr(self, 'visible', False),
            resizeable=False,
            left=10 | em,
            top=10 | em,
            content=Column(
                height=(auto, 0, 0),
                align_items=Alignment.Stretch,
                spacing=space,
                children=[
                    Row(
                        padding=(0 | em, 0 | em),
                        spacing=space,
                        align_items=Alignment.Stretch,
                        children=[cbutton(height=(s, 0, 0), width=(s, 0, 0), label=c) for c in row1]
                    ),
                    Row(
                        padding=(0 | em, 0 | em),
                        spacing=space,
                        align_items=Alignment.Stretch,
                        children=[cbutton(height=(s, 0, 0), width=(s, 0, 0), label=c) for c in row2]
                    ),
                    Row(
                        padding=(0 | em, 0 | em),
                        spacing=space,
                        align_items=Alignment.Stretch,
                        children=[cbutton(height=(s, 0, 0), width=(s, 0, 0), label=c) for c in row3]
                    ),
                    Row(
                        padding=(0 | em, 0 | em),
                        spacing=space,
                        align_items=Alignment.Stretch,
                        children=[Button(label='', height=(s, 0, 0), width=(s2, 0, 0))] + [
                            cbutton(height=(s, 0, 0), width=(s, 0, 0), label=c) for c in
                            row4]
                    )
                ]
            ))
        user_interface.add(self)

    async def push_input(self):
        await asyncio.sleep(0)
        await asyncio.sleep(0)
        await asyncio.sleep(0)
        for i in self._input:
            self.user_interface.add_input_character(i)
        self._input = []

    @staticmethod
    def install_on(user_interface):
        self = Keyboard(user_interface)

        async def update():
            if isinstance(user_interface.active_node, InputText):
                if user_interface.active_node is self.target:
                    for i in self._input:
                        self.user_interface.add_input_character(i)
                    self._input = []
                    return
                self.target = user_interface.active_node
                self.visible = True
                self.target.focus()

        user_interface.on_active_node_changed = update
