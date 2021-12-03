import skia

from matplotlib._pylab_helpers import Gcf

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

