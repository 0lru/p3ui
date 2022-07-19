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
        self.scale_min = (0.1, 0.1)
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
            on_mouse_leave=self.__on_mouse_leave,
            on_mouse_wheel=self.__on_mouse_wheel
        )
        self.__mouse_x = None
        self.__mouse_y = None

        super().__init__(*args, **kwargs,
                         padding=(0 | px, 0 | px),
                         content=self.__surface,
                         on_content_region_changed=self.__on_content_region_changed,
                         mouse_scroll_enabled=False)

    @property
    def scroll(self):
        """ set scroll coordinates without animation """
        return self.scroll_x, self.scroll_y

    @scroll.setter
    def scroll(self, scroll):
        """ get current scroll coordinates """
        self.scroll_x, self.scroll_y = scroll
        self.__update_surface()

    @property
    def scale(self):
        return self.__scale

    @scale.setter
    def scale(self, scale):
        if self.__fitting_mode is not ImageSurface.FittingMode.Custom:
            self.__fitting_mode = ImageSurface.FittingMode.Custom
            if self.__on_fitting_mode_changed:
                self.__on_fitting_mode_changed(self.__fitting_mode)
        e = 0.100001
        self.__scale = max(e, scale[0]), max(e, scale[1])
        self.__update_surface()
        if self.__on_scale_changed:
            self.__on_scale_changed(*scale)

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
        self.content_size = None
        if self.__on_scale_changed:
            self.__on_scale_changed(*self.__scale)

    def __run_animation(self):
        self.__animation_start_time = time.time()
        if self.__animation_task_instance is None:
            self.__animation_task_instance = asyncio.get_event_loop().create_task(self.__animation_task())

    def __on_mouse_enter(self, e):
        self.__mouse_x = e.x - self.scroll_x
        self.__mouse_y = e.y - self.scroll_y
        if self._on_mouse_enter:
            self._on_mouse_enter(self.mouse_x, self.mouse_y)

    def __on_mouse_move(self, e):
        lx = self.__mouse_x
        ly = self.__mouse_y
        self.__mouse_x = e.x - self.scroll_x
        self.__mouse_y = e.y - self.scroll_y
        dx = self.__mouse_x - lx
        dy = self.__mouse_y - ly
        #        print(dx, dy)
        if e.left_button_down:
            self.scroll = self.scroll_x - dx, self.scroll_y - dy
        if self._on_mouse_move:
            self._on_mouse_move(self.mouse_x, self.mouse_y)

    def __on_mouse_leave(self, e):
        self.__mouse_x = None
        self.__mouse_y = None
        if self._on_mouse_leave:
            self._on_mouse_leave(e.x, e.y)

    def __on_mouse_wheel(self, amount):
        self.zoom(amount * 0.5, amount * 0.5, point=(self.__mouse_x, self.__mouse_y))

    @property
    def mouse_x(self):
        if self.__mouse_x is None:
            return None
        if self.scale[0] == 0:
            return None
        return (self.__mouse_x + self.scroll_x) / self.scale[0]

    @property
    def mouse_y(self):
        if self.__mouse_y is None:
            return None
        if self.scale[1] == 0:
            return None
        return (self.__mouse_y + self.scroll_y) / self.scale[1]

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
        surface_width = int(self.image_width * self.__scale[0])
        surface_height = int(self.image_height * self.__scale[1])
        surface_width = max(surface_width, self.__content_region[2])
        surface_height = max(surface_height, self.__content_region[3])
        self.__surface.width = (surface_width | px, 0, 0)
        self.__surface.height = (surface_height | px, 0, 0)
        with self.__surface as canvas:
            canvas.save()
            canvas.scale(self.__scale[0], self.__scale[1])
            canvas.drawImage(self.__skia_image, 0, 0)
            canvas.restore()
            if self.__on_repaint is not None:
                self.__on_repaint(canvas, surface_width, surface_height)
        self.content_size = self.scaled_image_width, self.scaled_image_height

    @property
    def image(self):
        return self.__image

    @image.setter
    def image(self, image):
        self.__image = image
        self.__update_image()

    def scale_to(self, target_x, target_y):
        self.__scale_start = self.__scale[0], self.__scale[1]
        self.__scale_target = target_x, target_y
        if self.__fitting_mode is not ImageSurface.FittingMode.Custom:
            self.__fitting_mode = ImageSurface.FittingMode.Custom
            if self.__on_fitting_mode_changed:
                self.__on_fitting_mode_changed(self.__fitting_mode)
        self.__run_animation()

    async def __animate_task(self, scroll_start, scale_start, scroll_target, scale_target):
        now = time.time()
        start_time = now
        while now - start_time < self.__animation_duration:
            time_delta = now - start_time
            f = time_delta / self.__animation_duration
            self.__scale = (scale_start[0] + (scale_target[0] - scale_start[0]) * f,
                            scale_start[1] + (scale_target[1] - scale_start[1]) * f)
            self.scroll_x = scroll_start[0] + (scroll_target[0] - scroll_start[0]) * f
            self.scroll_y = scroll_start[1] + (scroll_target[1] - scroll_start[1]) * f
            self.__update_surface()
            if self.__on_scale_changed:
                self.__on_scale_changed(*self.__scale)
            await asyncio.sleep(0)
            now = time.time()

    def zoom(self, fx=None, fy=None, *, point=None):
        if self.__content_region is None:
            return
        scroll_x, scroll_y = self.scroll
        scale_x, scale_y = self.__scale
        if fx == -1.:
            fx = 0
        elif fx < 0:
            fx = fx / (1 + abs(fx))
        if fy == -1.:
            fy = 0
        elif fy < 0:
            fy = fy / (1 + abs(fy))
        scale_x_target, scale_y_target = scale_x + scale_x * fx, scale_y + scale_y * fy
        if scale_x_target < self.scale_min[0] or scale_y_target < self.scale_min[1]:
            return
        if point is None:
            _, _, w, h, bar = self.__content_region
            w = min(self.image_width * scale_x, w - bar)
            h = min(self.image_height * scale_y, h - bar)
            point = (w / 2, h / 2)
        scroll_x_target = (scroll_x + point[0]) / scale_x * scale_x_target - point[0] + 0.5
        scroll_y_target = (scroll_y + point[1]) / scale_y * scale_y_target - point[1] + 0.5
        self.scale = scale_x_target, scale_y_target
        self.scroll = scroll_x_target, scroll_y_target

    def zoom_in(self, fx=None, fy=None):
        if fx is None:
            fx = self.__scale_step_width
        if fy is None:
            fy = self.__scale_step_width
        if self.__content_region is None:
            return
        scroll_x, scroll_y = self.scroll
        scale_x, scale_y = self.__scale
        scale_x_target = scale_x + scale_x * fx
        scale_y_target = scale_y + scale_y * fy
        _, _, w, h, bar = self.__content_region
        w = min(self.image_width * scale_x, w - bar)
        h = min(self.image_height * scale_y, h - bar)
        scroll_x_target = (scroll_x + w / 2) / scale_x * scale_x_target - w / 2
        scroll_y_target = (scroll_y + h / 2) / scale_y * scale_y_target - h / 2
        self.scroll = scroll_x_target, scroll_y_target
        self.scale = scale_x_target, scale_y_target

    # factor 2, 2x2 bild:
    # *--*--*
    #    x x+w

    def zoom_out(self, fx=None, fy=None):
        if fx is None:
            fx = self.__scale_step_width
        if fy is None:
            fy = self.__scale_step_width
        if self.__content_region is None:
            return
        scroll_x, scroll_y = self.scroll
        scale_x, scale_y = self.__scale
        scale_x_target = scale_x - scale_x * (fx / (1 + fx))
        scale_y_target = scale_y - scale_y * (fy / (1 + fy))
        _, _, w, h, bar = self.__content_region
        w = min(self.image_width * scale_x, w)
        h = min(self.image_height * scale_y, h)
        scroll_x_target = (scroll_x + w / 2) / scale_x * scale_x_target - w / 2
        scroll_y_target = (scroll_y + h / 2) / scale_y * scale_y_target - h / 2
        self.scroll = scroll_x_target, scroll_y_target
        self.scale = scale_x_target, scale_y_target

    def set_scale_to_contain(self):
        self.fitting_mode = ImageSurface.FittingMode.Contain

    def set_no_scale(self):
        self.scale = (1., 1.)

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
    def __on_content_region_changed(self, viewport):
        self.__content_region = viewport
        self.__update_surface()

    #
    @property
    def displayed_image_x_range(self):
        """ returns min, max - tuple of image coordinates """
        if self.__image is None or self.__content_region is None:
            return 0, 0
        start, _, w, _, _ = self.__content_region
        end = start + w
        start /= self.scale[0]
        end /= self.scale[0]
        if end - start > self.image_width:
            return 0, self.image_width
        return start, end

    @property
    def displayed_image_y_range(self):
        """ returns min, max - tuple of image coordinates """
        if self.__image is None or self.__content_region is None:
            return 0, 0
        _, start, _, h, _ = self.__content_region
        end = start + h
        start /= self.scale[0]
        end /= self.scale[0]
        if end - start > self.image_height:
            return 0, self.image_height
        return start, end

    @property
    def viewport(self):
        """ returns x, y, w, h where x, y is the amount of scroll in [px]"""
        if self.__content_region is None:
            return None
        return self.__content_region[0:4]
