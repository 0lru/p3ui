from matplotlib.backend_bases import FigureManagerBase


class FigureManager(FigureManagerBase):
    """
    Helper class for pyplot mode, wraps everything up into a neat bundle.
    For non-interactive backends, the base class is sufficient.
    """