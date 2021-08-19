from p3ui import *


class TabTables(Table):

    def __init__(self):
        super().__init__(
            columns=[
                Table.Column('First Column', width=10 | em),
                Table.Column('Second Column', width=10 | em),
                Table.Column('Third Column', width=10 | em)
            ],
            children=[
                Table.Row(children=[Text('a111', color='red'), Text('222'), Text('333')]),
                Table.Row(children=[Text('b111'), Text('222', color='green'), Text('333')]),
                Table.Row(children=[Text('c111'), Text('222'), Text('333')])
            ]
        )
