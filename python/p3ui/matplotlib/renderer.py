import numpy as np
import skia
from matplotlib.backend_bases import RendererBase
from matplotlib.path import Path
from matplotlib.transforms import Affine2D

from .make_font import _make_font_from_properties
from .graphics_context import GraphicsContext


class Renderer(RendererBase):

    def __init__(self, width, height, dpi, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.dpi = dpi
        self.canvas = None
        self.width = width
        self.height = height

    def draw_path(self, gc, path, transform, rgbFace=None):
        self.canvas.save()
        transform = (transform + Affine2D().scale(1, -1).translate(0, self.height))
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
        paint = skia.Paint(Alpha=int(gc.get_alpha() * 255.), StrokeWidth=gc.get_linewidth())
        paint.setAntiAlias(True)
        if rgbFace is not None:
            paint.setColor(skia.Color4f(*rgbFace))
            paint.setStyle(skia.Paint.kFill_Style)
            self.canvas.drawPath(skia_path, paint)
        paint.setColor(skia.Color4f(gc.get_rgb()))
        paint.setStyle(skia.Paint.kStroke_Style)
        self.canvas.drawPath(skia_path, paint)
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
        font = _make_font_from_properties(properties)
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

    def get_text_width_height_descent(self, s, prop, ismath):
        font = _make_font_from_properties(prop)
        # TODO how to get h, b?
        return font.measureText(s), prop.get_size(), 1

    def new_gc(self):
        # docstring inherited
        return GraphicsContext()

    def points_to_pixels(self, points):
        return points / 72.0 * self.dpi