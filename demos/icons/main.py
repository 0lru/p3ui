from icon_provider import IconProvider
import asyncio
from p3ui import *


def make_paint_from_color(color):
    paint = skia.Paint()
    paint.setStrokeWidth(1.0)
    paint.setColor(color)
    return paint

font_size = 24
typeface = skia.Typeface('Consolas')
font = skia.Font(typeface, font_size, 1.0, 0.0)
paint = skia.Paint(AntiAlias=True, Color=skia.ColorBLACK)


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
        self._surface = Surface()
        super().__init__(children=[
            self._style_combo,
            ScrollArea(content=self._surface)
        ], **kwargs)
        self._update(0)

    def _update(self, index):
        style = self._style_combo.options[index]
        print(style)
        categories = self._provider.categories(style)
        with self._surface as canvas:
            canvas.clear(skia.ColorWHITE)
            x, y = 20, 20 + 16
            for category in categories:
                canvas.drawString(category, x, y, font, paint)
                x += 50
                y += font_size
                icons = self._provider.icons(style, category)
                for icon in icons:
                    dom = self._provider.load_icon(style, category, icon)
                    canvas.save()
                    canvas.translate(x, y + self.spacing)
                    canvas.scale(self.icon_size / 24, self.icon_size / 24)
                    dom.setContainerSize(skia.Size(self.icon_size, self.icon_size))
                    dom.render(canvas)
                    x += self.icon_size + self.spacing * 2
                    canvas.restore()
                    if x > 1100:
                        x = 70
                        y += self.icon_size + self.spacing * 2
                y += self.icon_size * 3
                x = 20

        self._surface.style.width = (1200 | px, 0, 0)
        self._surface.style.height = (y | px, 0, 0)


async def main():
    window = Window(title='Icons')
    window.position = (50, 50)
    window.size = (1000, 900)
    await window.serve(UserInterface(content=IconBrowser()))


asyncio.run(main())
