import asyncio
import time
from enum import IntEnum
import numpy as np
from ..native import *
from .. import skia
from .icons import Icons
import math
import cv2 as cv


def make_text_tooltip(text):
    return ToolTip(
        alpha=1,
        content=Row(
            width=(auto, 0 | px, 0 | px),
            height=(auto, 0 | px, 0 | px),
            children=[Text(text)]
        ))


def make_skia_image(image):
    if isinstance(image, np.ndarray):
        if len(image.shape) == 2:
            if image.dtype == np.uint8:
                return skia.Image.fromarray(image, skia.ColorType.kGray_8_ColorType)
        if len(image.shape) == 3:
            if image.shape[2] == 3:
                if image.dtype == np.uint8:
                    return skia.Image.fromarray(image.astype(np.uint8),
                                                skia.ColorType.kRGB_888x_ColorType)
            if image.shape[2] == 4:
                if image.dtype == np.uint8:
                    return skia.Image.fromarray(image.astype(np.uint8),
                                                skia.ColorType.kRGBA_8888_ColorType)
    raise RuntimeError('unknown image data format')


class ImageSurface(ScrollArea):
    class FittingMode(IntEnum):
        Custom = 0,
        Fill = 1,
        Contain = 2,
        Cover = 3

    class FeatureScaling(IntEnum):
        Original = 0,
        MinMax = 1,
        Logarithmic = 2

    class Painter:

        def __init__(self, surface, canvas):
            self.surface = surface
            self.canvas = canvas

        def draw_rectangle(self, x1, y1, x2, y2, color, stroke_width=1.):
            if not isinstance(color, Color):
                color = Color(color)
            paint = skia.Paint(
                AntiAlias=True,
                Color=skia.ColorSetARGB(color.alpha, color.red, color.green, color.blue),
                Style=skia.Paint.kStroke_Style,
                StrokeWidth=stroke_width
            )
            sx, sy = self.surface.scale_view
            self.canvas.drawRect(skia.Rect.MakeXYWH(x1 * sx, y1 * sy, (x2 - x1) * sx, (y2 - y1) * sy), paint)

        def draw_line(self, x1, y1, x2, y2, color, *, stroke_width=1.):
            if not isinstance(color, Color):
                color = Color(color)
            paint = skia.Paint(
                AntiAlias=True,
                Color=skia.ColorSetARGB(color.alpha, color.red, color.green, color.blue),
                Style=skia.Paint.kStroke_Style,
                StrokeWidth=stroke_width
            )
            sx, sy = self.surface.scale_view
            self.canvas.drawLine(x1 * sx, y1 * sy, x2 * sx, y2 * sy, paint)

    def __init__(self, *args, **kwargs):
        self.__on_scale_changed = kwargs.pop('on_scale_changed', None)
        self.__on_fitting_mode_changed = kwargs.pop('on_fitting_mode_changed', None)
        self.__on_feature_scaling_changed = kwargs.pop('on_feature_scaling_changed', None)
        self.__on_repaint = kwargs.pop('on_repaint', None)
        self.__scale_x = 1.
        self.__scale_x_view_start = 1.
        self.__scale_x_view_current = 1.
        self.__scale_y = 1.
        self.__scale_y_view_start = 1.
        self.__scale_y_view_current = 1.
        self.__scale_animation_start_time = None
        self.__scale_animation_duration = 0.2
        self.__scale_step_width = 0.1
        self.__animation_task_instance = None

        self.__skia_image = None
        self.__image = None

        self.__fitting_mode = ImageSurface.FittingMode.Contain
        self.__feature_scaling = ImageSurface.FeatureScaling.Original

        self.__content_region = None
        self.__surface = Surface()
        super().__init__(*args, **kwargs,
                         content=self.__surface,
                         on_content_region_changed=self.__on_viewport_changed)

    async def __animation_task(self):
        now = time.time()
        while now - self.__scale_animation_start_time < self.__scale_animation_duration:
            time_delta = now - self.__scale_animation_start_time
            f = math.log((time_delta / self.__scale_animation_duration) + 1.)
            f = time_delta / self.__scale_animation_duration
            self.__scale_x_view_current = self.__scale_x_view_start + (self.__scale_x - self.__scale_x_view_start) * f
            self.__scale_y_view_current = self.__scale_y_view_start + (self.__scale_y - self.__scale_y_view_start) * f
            self.__update_surface()
            await asyncio.sleep(0)
            now = time.time()
        self.__scale_x_view_current = self.__scale_x
        self.__scale_y_view_current = self.__scale_y
        self.__animation_task_instance = None

    def __run_animation(self):
        self.__scale_animation_start_time = time.time()
        if self.__animation_task_instance is None:
            self.__animation_task_instance = asyncio.get_event_loop().create_task(self.__animation_task())

    #
    # tier 1: update the numpy image

    def __update_image(self):
        if self.__image is None:
            self.__skia_image = None
            self.__update_surface()
            return
        if self.__feature_scaling is ImageSurface.FeatureScaling.Original:
            self.__skia_image = make_skia_image(self.__image)
        elif self.__feature_scaling is ImageSurface.FeatureScaling.MinMax:
            image = self.__image
            image_min, image_max = image.min(), image.max()
            n = image_max - image_min
            if n != 0:
                image = ((image - image_min) / n * 255.).astype(np.uint8)
            self.__skia_image = make_skia_image(image)
        elif self.__feature_scaling is ImageSurface.FeatureScaling.Logarithmic:
            image = np.log(self.__image + 1.)
            image_min, image_max = image.min(), image.max()
            # image_min, image_max, _, _ = cv.minMaxLoc(image)
            n = image_max - image_min
            if n != 0:
                image = ((image - image_min) / n * 255.)
            self.__skia_image = make_skia_image(image.astype(np.uint8))
        self.__update_surface()

    #
    # tier 2: update skia image
    def __derive_scale(self):
        w, h = self.width, self.height
        vw, vh = self.__content_region[2], self.__content_region[3]
        if self.__fitting_mode is ImageSurface.FittingMode.Fill:
            return vw / w, vh / h
        elif self.__fitting_mode is ImageSurface.FittingMode.Cover:
            vw -= self.__content_region[4]
            vh -= self.__content_region[4]
            s = max(vw / w, vh / h)
            return s, s
        elif self.__fitting_mode is ImageSurface.FittingMode.Contain:
            s = min(vw / w, vh / h)
            return s, s
        else:
            return self.__scale_x, self.__scale_y

    def __update_surface(self):
        if self.__skia_image is None:
            return
        if self.__content_region is None:
            return
        temp = self.__scale_x, self.__scale_y
        self.__scale_x, self.__scale_y = self.__derive_scale()
        if self.__on_scale_changed and (temp[0] != self.__scale_x or temp[1] != self.__scale_y):
            self.__on_scale_changed(self.scale)

        if self.__fitting_mode is not ImageSurface.FittingMode.Custom:
            self.__scale_x_view_current = self.__scale_x
            self.__scale_x_view_start = self.__scale_x
            self.__scale_y_view_current = self.__scale_y
            self.__scale_y_view_start = self.__scale_y
        self.__surface.style.width = (int(self.width * self.__scale_x_view_current) | px, 0, 0)
        self.__surface.style.height = (int(self.height * self.__scale_y_view_current) | px, 0, 0)
        with self.__surface as canvas:
            canvas.save()
            canvas.scale(self.__scale_x_view_current, self.__scale_y_view_current)
            canvas.drawImage(self.__skia_image, 0, 0)
            canvas.restore()
            if self.__on_repaint is not None:
                self.__on_repaint(ImageSurface.Painter(self, canvas))

    @property
    def image(self):
        return self.__image

    @image.setter
    def image(self, image):
        self.__image = image
        self.__update_image()

    def zoom_in(self):
        self.scale = self.__scale_x + self.__scale_step_width, self.__scale_y + self.__scale_step_width

    def zoom_out(self):
        self.scale = self.__scale_x - self.__scale_step_width, self.__scale_y - self.__scale_step_width

    def set_scale_to_contain(self):
        self.fitting_mode = ImageSurface.FittingMode.Contain

    def set_no_scale(self):
        self.scale = (1., 1.)

    @property
    def scale_view(self):
        return self.__scale_x_view_current, self.__scale_y_view_current

    @property
    def scale_x(self):
        return self.__scale_x

    @scale_x.setter
    def scale_x(self, value):
        self.scale = value, self.__scale_y

    @property
    def scale_y(self):
        return self.__scale_y

    @scale_y.setter
    def scale_y(self, value):
        self.scale = self.__scale_x, value

    @property
    def scale(self):
        return self.__scale_x, self.__scale_y

    @scale.setter
    def scale(self, scale):
        self.__scale_x_view_start = self.__scale_x_view_current
        self.__scale_y_view_start = self.__scale_y_view_current
        self.__scale_x, self.__scale_y = scale
        if self.__fitting_mode is not ImageSurface.FittingMode.Custom:
            self.__fitting_mode = ImageSurface.FittingMode.Custom
            if self.__on_fitting_mode_changed:
                self.__on_fitting_mode_changed(self.__fitting_mode)
        if self.__on_scale_changed:
            self.__on_scale_changed(self.scale)
        self.__run_animation()

    @property
    def fitting_mode(self):
        return self.__fitting_mode

    @fitting_mode.setter
    def fitting_mode(self, value: FittingMode):
        self.__fitting_mode = value
        self.__update_surface()
        if self.__on_fitting_mode_changed:
            self.__on_fitting_mode_changed(self.__fitting_mode)

    @property
    def feature_scaling(self):
        return self.__feature_scaling

    @feature_scaling.setter
    def feature_scaling(self, value):
        self.__feature_scaling = value
        self.__update_image()
        if self.__on_feature_scaling_changed:
            self.__on_feature_scaling_changed(self.__feature_scaling)

    @property
    def scale_step_width(self):
        return self.__scale_step_width

    @property
    def scaled_width(self):
        return self.width * self.__scale_x

    @property
    def scaled_height(self):
        return self.height * self.__scale_y

    @property
    def width(self):
        if self.__skia_image is None:
            return 0
        return self.__skia_image.width()

    @property
    def height(self):
        if self.__skia_image is None:
            return 0
        return self.__skia_image.height()

    #
    # scaling..
    def __on_viewport_changed(self, viewport):
        self.__content_region = viewport
        self.__update_surface()


class ImageViewer(Layout):

    def __init__(self, *, on_repaint=None, collapsed=True):
        self.__collapsed = collapsed
        self._image_surface = ImageSurface(
            padding=(2 | px, 2 | px),
            on_scale_changed=self.__on_scale_changed,
            on_fitting_mode_changed=self.__on_fitting_mode_changed,
            on_repaint=on_repaint
        )
        self._scale_x_input = InputDouble(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            min=0.1,
            max=10.,
            value=1,
            format='x=%g',
            on_change=lambda v: setattr(self._image_surface, 'scale_x', v),
            step=self._image_surface.scale_step_width)
        self._scale_y_input = InputDouble(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            min=0.1,
            max=10.,
            value=1,
            format='y=%g',
            on_change=lambda v: setattr(self._image_surface, 'scale_y', v),
            step=self._image_surface.scale_step_width)
        self._scale_to_contain_button = Button(
            label=f'{Icons.AspectRatio}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self._image_surface.set_scale_to_contain,
            disabled=True)
        # self._scale_to_contain_button.add(make_text_tooltip('scale image to be contained'))
        self._reset_scale_button = Button(
            label=f'{Icons.Filter}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self._image_surface.set_no_scale)
        # self._reset_scale_button.add(make_text_tooltip('reset scale'))
        self._scale_cover_button = Button(
            label=f'{Icons.PhotoAlbum}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self._image_surface.set_no_scale)
        # self._scale_cover_button.add(make_text_tooltip('scale to cover frame'))
        self._scale_fill_button = Button(
            label=f'{Icons.Photo}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self._image_surface.set_no_scale)
        # self._scale_fill_button.add(make_text_tooltip('scale to fill frame'))
        self._zoom_in_button = Button(
            label=f'{Icons.ZoomIn}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self._image_surface.zoom_in)
        # self._zoom_in_button.add(make_text_tooltip('zoom in'))
        self._zoom_out_button = Button(
            label=f'{Icons.ZoomOut}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self._image_surface.zoom_out)
        # self._zoom_out_button.add(make_text_tooltip('zoom out'))
        self._fitting_mode_combo_box = ComboBox(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            options=['custom scale', 'scale to fill', 'scale to contain', 'scale to cover'],
            selected_index=int(self._image_surface.fitting_mode),
            on_change=lambda index: setattr(self._image_surface, 'fitting_mode', ImageSurface.FittingMode(index))
        )
        self._feature_scaling_combo_box = ComboBox(
            visible=not collapsed,
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            options=['original colors', 'scale to fit', 'log(x+1)'],
            selected_index=0,
            on_change=lambda index: setattr(self._image_surface, 'feature_scaling', ImageSurface.FeatureScaling(index))
        )
        self.__collapsed_button = Button(
            label=f'{Icons.ChevronLeft}',
            width=(auto, 0, 0),
            height=(auto, 0, 0),
            on_click=self.toggle_collapsed)
        self._make_layout()
        self.collapsed = collapsed

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

    def _make_layout(self):
        super().__init__(
            direction=Direction.Horizontal,
            justify_content=Justification.Start,
            align_items=Alignment.Stretch,
            children=[
                Column(
                    width=(auto, 0, 0),
                    padding=(0 | px, 0 | px),
                    justify_content=Justification.Start,
                    align_items=Alignment.End,
                    height=(auto, 1, 0),
                    children=[
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
                    ]),
                self._image_surface
            ])

    @property
    def image(self):
        return self._image_surface.image

    @image.setter
    def image(self, image):
        self._image_surface.image = image

    def __on_scale_changed(self, scale):
        self._scale_x_input.value, self._scale_y_input.value = scale

    def __on_fitting_mode_changed(self, mode):
        self._fitting_mode_combo_box.selected_index = int(mode)
        self._scale_to_contain_button.disabled = mode is ImageSurface.FittingMode.Contain
