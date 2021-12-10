from icon_provider import IconProvider
import asyncio
from p3ui import *


def sliced(seq, slice_length):
    l = list(seq)
    for i in range(0, len(l), slice_length):
        yield l[i:i + slice_length]


def make_paint_from_color(color):
    paint = skia.Paint()
    paint.setStrokeWidth(1.0)
    paint.setColor(color)
    return paint


font_size = 24
typeface = skia.Typeface('Consolas')
font = skia.Font(typeface, font_size, 1.0, 0.0)
paint = skia.Paint(AntiAlias=True, Color=skia.ColorBLACK)


class Icon(Surface):

    def __init__(self, dom, base_size=24, pixel_size=32, *, color=Color('yellow')):
        self.color = skia.Color(*color.rgba)
        backbuffer = skia.Surface(pixel_size, pixel_size)
        super().__init__(
            width=(pixel_size | px, 0, 0),
            height=(pixel_size | px, 0, 0)
        )
        with backbuffer as canvas:
            canvas.scale(pixel_size / base_size, pixel_size / base_size)
            dom.setContainerSize(skia.Size(pixel_size, pixel_size))
            dom.render(canvas)
        image = backbuffer.makeImageSnapshot()
        with self as canvas:
            # filter = skia.ColorFilters.Blend(0x00FFFFFF, skia.BlendMode.kDstIn)
            filter = skia.ColorMatrixFilter.MakeLightingFilter(skia.ColorWHITE, self.color)
            paint = skia.Paint(ColorFilter=filter)
            canvas.drawImage(image, 0, 0, paint)


class IconBrowser(Column):

    def __init__(self, spacing=4, icon_size=72, **kwargs):
        self.spacing = 4
        self.icon_size = icon_size
        self._provider = IconProvider('icons.zip')

        self._style_combo = ComboBox(
            label='style',
            options=list(self._provider.styles()),
            selected_index=0,
            on_change=self._update
        )
        self._scroll_area = ScrollArea()
        super().__init__(children=[
            self._style_combo,
            self._scroll_area
        ], **kwargs)
        self._update(0)

    def _update(self, index):
        self._content_column = Column()
        self._scroll_area.content = self._content_column
        style = self._style_combo.options[index]
        categories = self._provider.categories(style)
        for category in categories:
            self._content_column.add(Row(children=[Text(category)]))
            icons = self._provider.icons(style, category)
            for slice in sliced(icons, 20):
                doms = [self._provider.load_icon(style, category, name) for name in slice]
                self._content_column.add(Row(padding=(0 | px, 0 | px), children=[
                    Icon(dom, 24, 32) for dom in doms
                ]))


async def main():
    window = Window(title='Icons')
    window.position = (50, 50)
    window.size = (1000, 900)
    await window.serve(UserInterface(content=IconBrowser()))


asyncio.run(main())
