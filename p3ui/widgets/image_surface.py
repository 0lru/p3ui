import asyncio
import time
from enum import IntEnum

from p3ui import *
import numpy as np


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
            sx, sy = self.surface.scale
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
            sx, sy = self.surface.scale
            self.canvas.drawLine(x1 * sx, y1 * sy, x2 * sx, y2 * sy, paint)

    def __init__(self, *args,
                 on_mouse_move=None,
                 on_mouse_leave=None,
                 on_mouse_enter=None,
                 **kwargs):
        self._on_mouse_move = on_mouse_move
        self._on_mouse_leave = on_mouse_leave
        self._on_mouse_enter = on_mouse_enter

        self.__on_scale_changed = kwargs.pop('on_scale_changed', None)
        self.__on_fitting_mode_changed = kwargs.pop('on_fitting_mode_changed', None)
        self.__on_feature_scaling_changed = kwargs.pop('on_feature_scaling_changed', None)
        self.__on_repaint = kwargs.pop('on_repaint', None)

        self.__scroll_start = None
        self.__scroll_target = None

        self.__scale = (1., 1.)
        self.__scale_start = None
        self.__scale_target = None
        self.__scale_step_width = 0.5

        self.__animation_start_time = None
        self.__animation_duration = 0.2
        self.__animation_task_instance = None

        self.__skia_image = None
        self.__image = None

        self.__fitting_mode = ImageSurface.FittingMode.Contain
        self.__feature_scaling = ImageSurface.FeatureScaling.Original

        self.__content_region = None
        self.__surface = Surface(
            on_mouse_enter=self.__on_mouse_enter,
            on_mouse_move=self.__on_mouse_move,
            on_mouse_leave=self.__on_mouse_leave
        )
        self.__mouse_x = None
        self.__mouse_y = None

        super().__init__(*args, **kwargs,
                         content=self.__surface,
                         on_content_region_changed=self.__on_viewport_changed)

    @property
    def scroll(self):
        """ set scroll coordinates without animation """
        return self.scroll_x, self.scroll_y

    @scroll.setter
    def scroll(self, scroll):
        """ get current scroll coordinates """
        self.scroll_x, self.scroll_y = scroll

    @property
    def scale(self):
        return self.__scale

    @scale.setter
    def scale(self, scale):
        self.__scale = scale

    def scroll_to(self, x, y):
        """ run animation from current scroll coordinates to target coordinates """
        self.__scroll_start = self.scroll
        self.__scroll_target = x, y
        self.__run_animation()

    def scale_to(self, target_x, target_y):
        self.__scale_start = self.__scale[0], self.__scale[1]
        self.__scale_target = target_x, target_y
        if self.__fitting_mode is not ImageSurface.FittingMode.Custom:
            self.__fitting_mode = ImageSurface.FittingMode.Custom
            if self.__on_fitting_mode_changed:
                self.__on_fitting_mode_changed(self.__fitting_mode)
        self.__run_animation()

    async def __animation_task(self):
        now = time.time()
        while now - self.__animation_start_time < self.__animation_duration:
            time_delta = now - self.__animation_start_time
            f = time_delta / self.__animation_duration
            if self.__scale_target is not None:
                self.__scale = (self.__scale_start[0] + (self.__scale_target[0] - self.__scale_start[0]) * f,
                                self.__scale_start[1] + (self.__scale_target[1] - self.__scale_start[1]) * f)
            if self.__scroll_target is not None:
                self.scroll_x = self.__scroll_start[0] + (self.__scroll_target[0] - self.__scroll_start[0]) * f
                self.scroll_y = self.__scroll_start[1] + (self.__scroll_target[1] - self.__scroll_start[1]) * f
            self.__update_surface()
            if self.__on_scale_changed:
                self.__on_scale_changed(*self.__scale)
            await asyncio.sleep(0)
            now = time.time()
        #
        # animation finished
        if self.__scroll_target is not None:
            self.scroll = self.__scroll_target
            self.__scroll_target = None
        if self.__scale_target is not None:
            self.__scale = self.__scale_target
            self.__scale_target = None
        self.__animation_task_instance = None
        if self.__on_scale_changed:
            self.__on_scale_changed(*self.__scale)

    def __run_animation(self):
        self.__animation_start_time = time.time()
        if self.__animation_task_instance is None:
            self.__animation_task_instance = asyncio.get_event_loop().create_task(self.__animation_task())

    def __on_mouse_enter(self, e):
        self.__mouse_x = e.x
        self.__mouse_y = e.y
        if self._on_mouse_enter:
            self._on_mouse_enter(self.mouse_x, self.mouse_y)

    def __on_mouse_move(self, e):
        self.__mouse_x = e.x
        self.__mouse_y = e.y
        if self._on_mouse_move:
            self._on_mouse_move(self.mouse_x, self.mouse_y)

    def __on_mouse_leave(self, e):
        self.__mouse_x = None
        self.__mouse_y = None
        if self._on_mouse_leave:
            self._on_mouse_leave(self.mouse_x, self.mouse_y)

    @property
    def mouse_x(self):
        if self.__mouse_x is None:
            return None
        return self.__mouse_x / self.__scale[0]

    @property
    def mouse_y(self):
        if self.__mouse_y is None:
            return None
        return self.__mouse_y / self.__scale[1]

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
        w, h = self.image_width, self.image_height
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
            return self.scale[0], self.scale[1]

    def __update_surface(self):
        if self.__skia_image is None:
            return
        if self.__content_region is None:
            return
        previous_scale = self.__scale
        self.__scale = self.__derive_scale()
        if self.__on_scale_changed and (previous_scale[0] != self.__scale[0] or previous_scale[1] != self.__scale[1]):
            self.__on_scale_changed(*self.scale)
        self.__surface.style.width = (int(self.image_width * self.__scale[0]) | px, 0, 0)
        self.__surface.style.height = (int(self.image_height * self.__scale[1]) | px, 0, 0)
        with self.__surface as canvas:
            canvas.save()
            canvas.scale(self.__scale[0], self.__scale[1])
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
        if self.content_region is None:
            return
        sx = self.__scale[0] + self.__scale[0] * self.__scale_step_width
        sy = self.__scale[1] + self.__scale[1] * self.__scale_step_width
        self.scale_to(sx, sy)

    def zoom_out(self):
        s = (self.__scale_step_width / (1 + self.__scale_step_width))
        sx = self.__scale[0] - self.__scale[0] * s
        sy = self.__scale[1] - self.__scale[1] * s
        self.scale_to(sx, sy)

    def set_scale_to_contain(self):
        self.fitting_mode = ImageSurface.FittingMode.Contain

    def set_no_scale(self):
        self.scale_to(1., 1.)

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
    def scaled_image_width(self):
        return self.image_width * self.__scale[0]

    @property
    def scaled_image_height(self):
        return self.image_height * self.__scale[1]

    @property
    def image_width(self):
        if self.__skia_image is None:
            return 0
        return self.__skia_image.width()

    @property
    def image_height(self):
        if self.__skia_image is None:
            return 0
        return self.__skia_image.height()

    #
    # scaling..
    def __on_viewport_changed(self, viewport):
        self.__content_region = viewport
        self.__update_surface()
