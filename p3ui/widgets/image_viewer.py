from ..native import *
from .. import skia
from .image_surface import ImageSurface
from .icons import Icons


def make_text_tooltip(text):
    return ToolTip(
        alpha=1,
        content=Row(
            width=(auto, 0 | px, 0 | px),
            height=(auto, 0 | px, 0 | px),
            children=[Text(text)]
        ))


class ImageViewer(Layout):

    def __init__(self, *,
                 on_repaint=None,
                 collapsed=True,
                 on_mouse_move=None,
                 on_mouse_leave=None,
                 on_mouse_enter=None,
                 **kwargs):
        self.on_repaint = on_repaint
        self.__collapsed = collapsed
        self.image_surface = ImageSurface(
            on_mouse_enter=on_mouse_enter,
            on_mouse_move=on_mouse_move,
            on_mouse_leave=on_mouse_leave,
            on_scale_changed=self.__on_scale_changed,
            on_fitting_mode_changed=self.__on_fitting_mode_changed,
            on_repaint=self.__on_repaint,
        )
        self.vertical_plot = Plot(
            width=(11 | em, 0, 0),
            legend_visible=False,
            x_ticks_visible=False,
            y_inverted=True,
            x_opposite=True,
            y_opposite=True,
            line_weight=3.0,
            border_width=1 | px,
            padding=(0 | px, 0 | px)
        )
        self.horizontal_plot = Plot(
            width=(auto, 1, 1),
            height=(7 | em, 0, 0),
            legend_visible=False,
            line_weight=3.0,
            x_opposite=False,
            y_opposite=False,
            y_ticks_visible=False,
            border_width=1 | px,
            padding=(0 | px, 0 | px)
        )
        self._scale_x_input = InputDouble(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            min=0.1,
            max=10.,
            value=1,
            format='x=%g',
            on_change=self.__scale_to_x,
            step=0.3)
        self._scale_y_input = InputDouble(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            min=0.1,
            max=10.,
            value=1,
            format='y=%g',
            on_change=self.__scale_to_y,
            step=0.3)
        self._scale_to_contain_button = Button(
            label=f'{Icons.AspectRatio}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.image_surface.set_scale_to_contain,
            disabled=True)
        # self._scale_to_contain_button.add(make_text_tooltip('scale image to be contained'))
        self._reset_scale_button = Button(
            label=f'{Icons.Filter}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.image_surface.set_no_scale)
        # self._reset_scale_button.add(make_text_tooltip('reset scale'))
        self._scale_cover_button = Button(
            label=f'{Icons.PhotoAlbum}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.image_surface.set_no_scale)
        # self._scale_cover_button.add(make_text_tooltip('scale to cover frame'))
        self._scale_fill_button = Button(
            label=f'{Icons.Photo}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.image_surface.set_no_scale)
        # self._scale_fill_button.add(make_text_tooltip('scale to fill frame'))
        self._zoom_in_button = Button(
            label=f'{Icons.ZoomIn}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.image_surface.zoom_in)
        # self._zoom_in_button.add(make_text_tooltip('zoom in'))
        self._zoom_out_button = Button(
            label=f'{Icons.ZoomOut}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.image_surface.zoom_out)
        # self._zoom_out_button.add(make_text_tooltip('zoom out'))
        self._fitting_mode_combo_box = ComboBox(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            options=['custom scale', 'scale to fill', 'scale to contain', 'scale to cover'],
            selected_index=int(self.image_surface.fitting_mode),
            on_change=lambda index: setattr(self.image_surface, 'fitting_mode', ImageSurface.FittingMode(index))
        )
        self._feature_scaling_combo_box = ComboBox(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            options=['original colors', 'scale to fit', 'log(x+1)'],
            selected_index=0,
            on_change=lambda index: setattr(self.image_surface, 'feature_scaling', ImageSurface.FeatureScaling(index))
        )
        self.__collapsed_button = Button(
            label=f'{Icons.ChevronLeft}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.toggle_collapsed)
        self.collapsed = collapsed
        super().__init__(
            **kwargs,
            direction=Direction.Horizontal,
            justify_content=Justification.Start,
            align_items=Alignment.Stretch,
            children=[
                #
                # button column
                Column(
                    width=(auto, 0, 0),
                    padding=(0 | px, 0 | px),
                    justify_content=Justification.Start,
                    align_items=Alignment.End,
                    height=(auto, 1, 0),
                    children=[
                        #
                        # collapse button floating to the left
                        Row(
                            padding=(0 | px, 0 | px),
                            width=(100 | percent, 0, 0),
                            height=(auto, 0, 0),
                            justify_content=Justification.Start,
                            align_items=Alignment.Start,
                            children=[self.__collapsed_button]
                        ),
                        Text(''),  # force spacing of text height
                        self._feature_scaling_combo_box,
                        Text(''),  # force spacing of text height
                        self._scale_to_contain_button,
                        self._reset_scale_button,
                        self._zoom_in_button,
                        self._zoom_out_button,
                        Row(),
                        self._fitting_mode_combo_box,
                        self._scale_x_input,
                        self._scale_y_input
                    ]
                ),
                #
                # surface with plots besides
                Column(padding=(0 | px, 0 | px), spacing=0 | px, children=[
                    Row(
                        spacing=0 | px,
                        padding=(0 | px, 0 | px),
                        children=[
                            self.image_surface,
                            self.vertical_plot
                        ]
                    ),
                    Row(
                        spacing=0 | px,
                        padding=(0 | px, 0 | px),
                        height=(auto, 0, 0),
                        children=[self.horizontal_plot]
                    )
                ])
            ])

    @property
    def mouse_x(self):
        return self.image_surface.mouse_x

    @property
    def mouse_y(self):
        return self.image_surface.mouse_y

    @property
    def collapsed(self):
        return self.__collapsed

    @collapsed.setter
    def collapsed(self, value):
        self.__collapsed = value
        self._fitting_mode_combo_box.visible = not value
        self._feature_scaling_combo_box.visible = not value
        self._scale_x_input.visible = not value
        self._scale_y_input.visible = not value
        self.__collapsed_button.label = f'{Icons.ChevronRight}' if self.collapsed else f'{Icons.ChevronLeft}'

    def toggle_collapsed(self):
        self.collapsed = not self.collapsed

    @property
    def image(self):
        return self.image_surface.image

    @image.setter
    def image(self, image):
        self.image_surface.image = image

    def __scale_to_x(self, x):
        self.image_surface.scale = (x, self.image_surface.scale[1])
        self._scale_x_input.value = self.image_surface.scale[0]

    def __scale_to_y(self, y):
        self.image_surface.scale = (self.image_surface.scale[0], y)
        self._scale_y_input.value = self.image_surface.scale[1]

    def __on_scale_changed(self, x, y):
        self._scale_x_input.value, self._scale_y_input.value = x, y

    def __on_fitting_mode_changed(self, mode):
        self._fitting_mode_combo_box.selected_index = int(mode)
        self._scale_to_contain_button.disabled = mode is ImageSurface.FittingMode.Contain

    @property
    def scale(self):
        return self.image_surface.scale

    @scale.setter
    def scale(self, scale):
        self.image_surface.scale = scale

    @property
    def scale_x(self):
        return self.scale[0]

    @scale_x.setter
    def scale_x(self, x):
        self.scale = x, self.scale[1]

    @property
    def scale_y(self):
        return self.scale[1]

    @scale_y.setter
    def scale_y(self, y):
        self.scale = self.scale[0], y

    @property
    def scroll(self):
        return self.image_surface.scroll

    @scroll.setter
    def scroll(self, scroll):
        self.image_surface.scroll = scroll

    @property
    def displayed_image_y_range(self):
        return self.image_surface.displayed_image_y_range

    @property
    def displayed_image_x_range(self):
        return self.image_surface.displayed_image_x_range

    @property
    def viewport(self):
        return self.image_surface.viewport

    def __on_repaint(self, canvas, surface_width, surface_height):
        viewport = self.viewport
        if viewport is not None:
            w, h = self.image_surface.size
            self.vertical_plot.y_axis.fixed = True
            self.vertical_plot.y_axis.auto_fit = False
            self.vertical_plot.y_axis.limits = (
                viewport[1] / self.scale[1],
                (viewport[1] + h) / self.scale[1]
            )
            size = self.horizontal_plot.size
            self.horizontal_plot.x_axis.fixed = True
            self.horizontal_plot.x_axis.auto_fit = False
            self.horizontal_plot.x_axis.limits = (
                viewport[0] / self.scale[0],
                (viewport[0] + size[0]) / self.scale[0]
            )
        if self.on_repaint:
            self.on_repaint(canvas, surface_width, surface_height)
