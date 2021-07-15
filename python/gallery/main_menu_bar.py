from oogui import MenuBar, Menu, MenuItem


class MainMenuBar(MenuBar):

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
                on_change=lambda checked: print(f'menu checked: {checked}')
            ), Menu(
                'Sub',
                children=[MenuItem(
                    'Item'
                )]
            )]
        ))

        self.add(Menu(
            'View',
            children=[
                MenuItem('Any')
            ]
        ))

