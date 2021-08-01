from p3ui import MenuBar, Menu, MenuItem


class MenuBar(MenuBar):

    def __init__(self):
        super().__init__()
        self.add(Menu(
            'File',
            on_open=lambda: print('file menu opened'),
            on_close=lambda: print('file menu closed'),
            children=[MenuItem(
                'Open',
                shortcut='CTRL+O',
                on_click=lambda: print('menu item clicked')
            ), MenuItem(
                'Toggle',
                checkable=True,
                on_change=lambda value: print(f'menu checked: {value}')
            ), Menu(
                'Sub',
                children=[MenuItem(
                    'Item'
                )]
            )]
        ))

