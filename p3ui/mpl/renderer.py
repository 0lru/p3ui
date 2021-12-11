import numpy as np
import p3ui.skia as skia
from matplotlib.backend_bases import RendererBase
from matplotlib.path import Path

from .make_font import _make_font_from_properties
from .graphics_context import GraphicsContext
from ._units import dpi as default_dpi


class Renderer(RendererBase):

    def __init__(self, width, height, dpi=default_dpi, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.dpi = dpi
        self.canvas = None
        self.width = width
        self.height = height

    def draw_path(self, gc, path, transform, rgbFace=None):
        self.canvas.save()
        self.canvas.translate(0, self.height)
        self.canvas.scale(1, -1)
        skia_path = skia.Path()
        for points, code in path.iter_segments(transform, remove_nans=True, clip=None):  # TODO clip=clip
            if code == Path.MOVETO:
                skia_path.moveTo(*points)
            elif code == Path.CLOSEPOLY:
                skia_path.close()
            elif code == Path.LINETO:
                skia_path.lineTo(*points)
            elif code == Path.CURVE3:
                skia_path.quadTo(*points)
            elif code == Path.CURVE4:
                skia_path.cubicTo(*points)
        if rgbFace is not None:
            gc.paint.setColor(skia.Color4f(*rgbFace))
            gc.paint.setStyle(skia.Paint.kFill_Style)
            self.canvas.drawPath(skia_path, gc.paint)
        if gc.get_linewidth() > 0:
            gc.paint.setColor(skia.Color4f(gc.get_rgb()))
            gc.paint.setStyle(skia.Paint.kStroke_Style)
            self.canvas.drawPath(skia_path, gc.paint)
        self.canvas.restore()

    # draw_markers is optional, and we get more correct relative
    # timings by leaving it out.  backend implementers concerned with
    # performance will probably want to implement it
    #     def draw_markers(self, gc, marker_path, marker_trans, path, trans,
    #                      rgbFace=None):
    #         pass

    # draw_path_collection is optional, and we get more correct
    # relative timings by leaving it out. backend implementers concerned with
    # performance will probably want to implement it
    #     def draw_path_collection(self, gc, master_transform, paths,
    #                              all_transforms, offsets, offsetTrans,
    #                              facecolors, edgecolors, linewidths, linestyles,
    #                              antialiaseds):
    #         pass

    # draw_quad_mesh is optional, and we get more correct
    # relative timings by leaving it out.  backend implementers concerned with
    # performance will probably want to implement it
    #     def draw_quad_mesh(self, gc, master_transform, meshWidth, meshHeight,
    #                        coordinates, offsets, offsetTrans, facecolors,
    #                        antialiased, edgecolors):
    #         pass

    def draw_image(self, gc, x, y, im):
        # docstring inherited
        temp = np.flip(im, axis=0).copy()
        image = skia.Image.fromarray(temp, skia.ColorType.kRGBA_8888_ColorType)
        self.canvas.drawImage(image, x, self.height - y - temp.shape[0])

    def draw_text(self, gc, x, y, s, properties, angle, ismath=False, mtext=None):
        # docstring inherited
        self.canvas.save()
        paint = skia.Paint(AntiAlias=True, Color=skia.Color4f(gc.get_rgb()))
        font = _make_font_from_properties(properties, self.points_to_pixels(properties.get_size_in_points()))
        self.canvas.translate(x, self.height - y)
        if angle:
            self.canvas.rotate(-angle)
        self.canvas.drawString(s, 0, 0, font, paint)
        self.canvas.restore()

    def flipy(self):
        # docstring inherited
        return False

    def get_canvas_width_height(self):
        # docstring inherited
        return self.width, self.height

    def get_text_width_height_descent(self, s, properties, ismath):
        height_pixels = self.points_to_pixels(properties.get_size_in_points())
        font = _make_font_from_properties(properties, height_pixels)
        #
        # in reality, the font may be a few pixels smaller in height
        return font.measureText(s), height_pixels, font.getMetrics().fDescent

    def new_gc(self):
        # docstring inherited
        return GraphicsContext()

    def points_to_pixels(self, points):
        return points * self.dpi / 72
