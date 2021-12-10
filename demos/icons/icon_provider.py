import zipfile
from pathlib import Path
from p3ui import skia


class IconProvider:
    class Error(Exception):
        pass

    class Descriptor:

        def __init__(self, path):
            self.category, self.name, self.style, self.filename = path.parts[0:4]
            self.style = self.style[IconProvider._style_prefix_length:]
            if len(self.style) == 0:
                self.style = 'normal'
            self.path = path

    _style_prefix_length = len('materialicons')

    def __init__(self, zip_filename, *, buffered=True):
        if buffered:
            self._buff = dict()
        self._hash = dict()
        self._by_name = dict()
        self._by_category = dict()
        self._file = zipfile.ZipFile(zip_filename)
        for item in self._file.namelist():
            path = Path(item)
            if path.suffix.lower() == '.svg':
                self._import_path(path)

    def _import_path(self, path):
        descriptor = IconProvider.Descriptor(path)
        self._build_by_name_hash(descriptor)
        self._build_by_category_hash(descriptor)

    def _build_by_category_hash(self, descriptor):
        if descriptor.style not in self._by_category:
            self._by_category[descriptor.style] = dict()

        style_dict = self._by_category[descriptor.style]

        if descriptor.category not in style_dict:
            style_dict[descriptor.category] = dict()

        named_dict = style_dict[descriptor.category]
        if descriptor.name not in named_dict:
            named_dict[descriptor.name] = []

        bucket = named_dict[descriptor.name]
        result = next(filter(lambda item: item.filename == descriptor.filename, bucket), None)
        if result is not None:
            raise IconProvider.Error(f'error adding: {descriptor.path}')
        bucket.append(descriptor)

    def _build_by_name_hash(self, descriptor):

        if descriptor.style not in self._by_name:
            self._by_name[descriptor.style] = dict()

        style_dict = self._by_name[descriptor.style]

        if descriptor.name not in style_dict:
            style_dict[descriptor.name] = []

        bucket = style_dict[descriptor.name]

        result = next(filter(lambda item: item.filename == descriptor.filename, bucket), None)
        if result is not None:
            raise IconProvider.Error(f'error adding: {descriptor.path}')
        bucket.append(descriptor)

    def styles(self):
        return self._by_name.keys()

    def names(self, style):
        return self._by_name[style].keys()

    def categories(self, style):
        return self._by_category[style].keys()

    def icons(self, style, category):
        return self._by_category[style][category].keys()

    def load_icon(self, style, category, name):
        bucket = self._by_category[style][category][name]
        if bucket is None:
            raise IconProvider.Error(f'icon not found ({style}:{category}:{name})')
        icon = bucket[-1]
        bytes = None
        if self._buff and icon in self._buff:
            bytes = self._buff[icon]
        else:
            bytes = self._file.read(icon.path.as_posix())
            if self._buff:
                self._buff[icon] = bytes
        stream = skia.MemoryStream(bytes, False)
        return skia.SVGDOM.MakeFromStream(stream)
