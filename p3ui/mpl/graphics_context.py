from matplotlib.backend_bases import GraphicsContextBase
from .. import skia as skia


class GraphicsContext(GraphicsContextBase):

    def __init__(self):
        super().__init__()
        self.paint = skia.Paint()
        self.path_effect = None

    _cap_dict = {'butt': skia.Paint.kButt_Cap,
                 'projecting': skia.Paint.kDefault_Cap,
                 'round': skia.Paint.kRound_Cap}

    _join_dict = {'bevel': skia.Paint.kBevel_Join,
                  'miter': skia.Paint.kMiter_Join,
                  'round': skia.Paint.kRound_Join}

    def set_foreground(self, fg, isRGBA=None):
        super().set_foreground(fg, isRGBA)
        self.paint.setColor(skia.Color4f(self.get_rgb()))

    def set_alpha(self, alpha):
        super().set_alpha(alpha)
        self.paint.setAlpha(int(self.get_alpha() * 255.))

    def set_dashes(self, offset, dashes):
        if dashes is not None:
            self.paint.setPathEffect(skia.DashPathEffect.Make(dashes, offset))

    def set_antialiased(self, flag):
        self.paint.setAntiAlias(flag)

    def set_linewidth(self, w):
        # docstring inherited
        if 0 < w < 1:
            w = 1.
        self.paint.setStrokeWidth(w)

    def set_capstyle(self, cs):
        # docstring inherited
        self.paint.setStrokeCap(GraphicsContext._cap_dict[cs])

    def set_joinstyle(self, js):
        # docstring inherited
        self.paint.setStrokeJoin(GraphicsContext._join_dict[js])
