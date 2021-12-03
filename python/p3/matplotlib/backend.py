"""
This provides a starting point for backend writers; you can
selectively implement drawing methods (`~.RendererTemplate.draw_path`,
`~.RendererTemplate.draw_image`, etc.) and slowly see your figure come to life
instead having to have a full blown implementation before getting any results.

If your backend implements support for saving figures (i.e. has a `print_xyz`
method), you can register it as the default handler for a given file type::
    from matplotlib.backend_bases import register_backend
    register_backend('xyz', 'my_backend', 'XYZ File Format')
    ...
    plt.savefig("figure.xyz")
"""
import skia
import numpy as np
from matplotlib.path import Path

from matplotlib import _api
from matplotlib._pylab_helpers import Gcf
from matplotlib.backend_bases import (
    FigureCanvasBase, FigureManagerBase, GraphicsContextBase, RendererBase)
from matplotlib.figure import Figure
from matplotlib.transforms import Affine2D

_weight_dict = {
    #    0: skia.FontStyle.kInvisible_Weight,
    #    100: skia.FontStyle.kThin_Weight,
    #    200: skia.FontStyle.kExtraLight_Weight,
    #    300: skia.FontStyle.kLight_Weight,
    #    400: skia.FontStyle.kNormal_Weight,
    #    500: skia.FontStyle.kMedium_Weight,
    #    600: skia.FontStyle.kSemiBold_Weight,
    #    700: skia.FontStyle.kBold_Weight,
    #    800: skia.FontStyle.kExtraBold_Weight,
    #    900: skia.FontStyle.kBlack_Weight,
    #    1000: skia.FontStyle.kExtraBlack_Weight,
    'thin': skia.FontStyle.kExtraLight_Weight,  # 100
    'ultralight': skia.FontStyle.kExtraLight_Weight,  # 200
    'light': skia.FontStyle.kLight_Weight,  # 300
    'regular': skia.FontStyle.kNormal_Weight,  # 400
    'normal': skia.FontStyle.kNormal_Weight,  # 400
    'medium': skia.FontStyle.kMedium_Weight,  # 500
    'semibold': skia.FontStyle.kSemiBold_Weight,  # 600
    'bold': skia.FontStyle.kBold_Weight,  # 700
    'ultrabold': skia.FontStyle.kExtraBold_Weight,  # 800
    'black': skia.FontStyle.kBlack_Weight,  # 900
    'heavy': skia.FontStyle.kExtraBlack_Weight,  # 1000
}

_slant_dict = {
    'italic': skia.FontStyle.kItalic_Slant,
    'normal': skia.FontStyle.kUpright_Slant,
    'oblique': skia.FontStyle.kOblique_Slant,
}


# kUltraCondensed_Width = 1, kExtraCondensed_Width = 2, kCondensed_Width = 3, kSemiCondensed_Width = 4,
# kNormal_Width = 5, kSemiExpanded_Width = 6, kExpanded_Width = 7, kExtraExpanded_Width = 8,
# kUltraExpanded_Width = 9


def _make_font(name, weight, slant, point_size):
    if weight in _weight_dict:
        weight = _weight_dict[weight]
    style = skia.FontStyle(weight, 5, _slant_dict[slant])
    font = skia.Font(skia.Typeface(name, style), point_size)
    return font


def _make_font_from_properties(properties):
    return _make_font(properties.get_name(), properties.get_weight(), properties.get_style(),
                      properties.get_size_in_points())


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


class GraphicsContext(GraphicsContextBase):

    def __init__(self):
        super().__init__()


########################################################################
#
# The following functions and classes are for pyplot and implement
# window/figure managers, etc...
#
########################################################################


def draw_if_interactive():
    """
    For image backends - is not required.
    For GUI backends - this should be overridden if drawing should be done in
    interactive python mode.
    """


def show(*, block=None):
    print('called show')
    """
    For image backends - is not required.
    For GUI backends - show() is usually the last line of a pyplot script and
    tells the backend that it is time to draw.  In interactive mode, this
    should do nothing.
    """
    for manager in Gcf.get_all_fig_managers():
        # do something to display the GUI
        pass


def new_figure_manager(num, *args, FigureClass=Figure, **kwargs):
    """Create a new figure manager instance."""
    # If a main-level app must be created, this (and
    # new_figure_manager_given_figure) is the usual place to do it -- see
    # backend_wx, backend_wxagg and backend_tkagg for examples.  Not all GUIs
    # require explicit instantiation of a main-level app (e.g., backend_gtk3)
    # for pylab.
    thisFig = FigureClass(*args, **kwargs)
    return new_figure_manager_given_figure(num, thisFig)


def new_figure_manager_given_figure(num, figure):
    """Create a new figure manager instance for the given figure."""
    canvas = FigureCanvas(figure)
    manager = FigureManager(canvas, num)
    return manager


class FigureCanvas(FigureCanvasBase):
    """
    The canvas the figure renders into.  Calls the draw and print fig
    methods, creates the renderers, etc.
    Note: GUI templates will want to connect events for button presses,
    mouse movements and key presses to functions that call the base
    class methods button_press_event, button_release_event,
    motion_notify_event, key_press_event, and key_release_event.  See the
    implementations of the interactive backends for examples.
    Attributes
    ----------
    figure : `matplotlib.figure.Figure`
        A high-level Figure instance
    """

    def __init__(self, *args, **kwargs):
        super(FigureCanvas, self).__init__(*args, **kwargs)

    def draw(self):
        """
        Draw the figure using the renderer.
        It is important that this method actually walk the artist tree
        even if not output is produced because this will trigger
        deferred work (like computing limits auto-limits and tick
        values) that users may want access to before saving to disk.
        """
        print('draw')
        renderer = Renderer(self.figure.dpi)
        self.figure.draw(renderer)

    # You should provide a print_xxx function for every file format
    # you can write.

    # If the file type is not in the base set of filetypes,
    # you should add it to the class-scope filetypes dictionary as follows:
    filetypes = {**FigureCanvasBase.filetypes, 'foo': 'My magic Foo format'}

    @_api.delete_parameter("3.5", "args")
    def print_foo(self, filename, *args, **kwargs):
        """
        Write out format foo.
        This method is normally called via `.Figure.savefig` and
        `.FigureCanvasBase.print_figure`, which take care of setting the figure
        facecolor, edgecolor, and dpi to the desired output values, and will
        restore them to the original values.  Therefore, `print_foo` does not
        need to handle these settings.
        """
        self.draw()

    def get_default_filetype(self):
        return 'foo'


class FigureManager(FigureManagerBase):
    """
    Helper class for pyplot mode, wraps everything up into a neat bundle.
    For non-interactive backends, the base class is sufficient.
    """
