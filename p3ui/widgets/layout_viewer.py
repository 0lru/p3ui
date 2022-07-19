class Spacer(Row):

    def __init__(self, width=(auto, 0, 0), height=(auto, 0, 0)):
        super().__init__(
            width=width,
            height=height,
            padding=(0 | px, 0 | px),
            spacing=0 | px
        )


class LayoutEditor(ChildWindow):
    class TreeNode(Column):

        def __init__(self, node):
            self.indent = Spacer(width=(1 | em, 0, 0))
            self.node_type = Text('')
            self.node_label = Text('', color='#440000')
            self.node_value = Text('', color='#227722')
            self.line = Row(
                width=(auto, 1, 0),
                justify_content=Justification.Start,
                height=(auto, 0, 0),
                children=[self.indent, self.node_type, self.node_label, self.node_value]
            )
            self.container = Column(
                padding=(0 | em, 0 | em),
                spacing=0 | px,
                height=(auto, 0, 0),
            )
            super().__init__(
                padding=(0 | em, 0 | em),
                spacing=0 | px,
                width=(auto, 0, 0),
                height=(auto, 0, 0),
                align_items=Alignment.Stretch,
                children=[
                    self.line,
                    self.container
                ]
            )
            self.node = node
            self.node_type.value = f'{Icons.KeyboardArrowRight} {type(node).__name__:<20}'
            self.node_label.value = node.label if node.label is not None else ''
            self.node_value.value = str(node.value) if hasattr(node, 'value') else ''

    def __init__(self, window):
        self.window = window
        self.tree = None
        self.scroll_area = ScrollArea()
        super().__init__(
            label='Layout Editor',
            width=(40 | em, 0, 0),
            height=(30 | em, 0, 0),
            content=self.scroll_area
        )
        self.is_odd = False

    def __update(self, tree_node, node, even_index=True, indent=0):
        self.is_odd = not self.is_odd
        if tree_node.node != node:
            tree_node = LayoutEditor.TreeNode(node)
        if self.is_odd:
            tree_node.line.background_color = '#00000000'
        else:
            tree_node.line.background_color = '#ffffffff'
        tree_node.indent.width = (indent | em, 0, 0)
        for index, child in enumerate(node.children):
            if child is self:
                continue
            if index < len(tree_node.container.children):
                tree_node.container.children[index] = self.__update(
                    tree_node.container.children[index],
                    child,
                    even_index,
                    indent + 1
                )
            else:
                temp = LayoutEditor.TreeNode(child)
                tree_node.container.add(temp)
                self.__update(
                    temp,
                    child,
                    even_index,
                    indent + 1
                )
        return tree_node

    def update(self):
        self.is_odd = False
        if self.tree is None:
            self.tree = LayoutEditor.TreeNode(self.window.user_interface)
        self.tree = self.__update(self.tree, self.window.user_interface)
        self.scroll_area.content = self.tree
