import asyncio
from p3ui import *


def draw_text(canvas):
    typeface = skia.Typeface('Arial')

    font1 = skia.Font(typeface, 64.0, 1.0, 0.0)
    font2 = skia.Font(typeface, 64.0, 1.5, 0.0)
    font1.setEdging(skia.Font.Edging.kAntiAlias)
    font2.setEdging(skia.Font.Edging.kAntiAlias)

    blob1 = skia.TextBlob.MakeFromString('Skia', font1)
    blob2 = skia.TextBlob.MakeFromString('Skia', font2)

    paint1 = skia.Paint(
        AntiAlias=True,
        Color=skia.Color(0x42, 0x85, 0xF4))

    paint2 = skia.Paint(
        AntiAlias=True,
        Color=skia.Color(0xDB, 0x44, 0x37),
        Style=skia.Paint.kStroke_Style,
        StrokeWidth=3.0)

    paint3 = skia.Paint(
        AntiAlias=True,
        Color=skia.Color(0x0F, 0x9D, 0x58))

    canvas.drawTextBlob(blob1, 20.0, 64.0, paint1)
    canvas.drawTextBlob(blob1, 20.0, 144.0, paint2)
    canvas.drawTextBlob(blob2, 20.0, 224.0, paint3)


class CanvasDemo(ScrollArea):

    def __init__(self):
        super().__init__()
        self.surface = Surface(
            width=(100 | em, 0, 0),
            height=(100 | em, 0, 0)
        )
        self.content = self.surface

    @staticmethod
    def star():
        """
        @see https://github.com/kyamagu/skia-python/blob/main/notebooks/Showcase.ipynb
        """
        from math import cos, sin
        radius = 100.0
        center = 128.0
        path = skia.Path()
        path.moveTo(center + radius, center)
        for i in range(1, 8):
            a = 2.6927937 * i
            path.lineTo(center + radius * cos(a), center + radius * sin(a))
        return path

    async def update(self):
        rotation = 0
        distort = 5
        while True:
            await asyncio.sleep(0.01)
            with self.surface as canvas:
                canvas.save()
                canvas.translate(50, 50)
                canvas.rotate(rotation, 128, 128)
                paint = skia.Paint(
                    PathEffect=skia.DiscretePathEffect.Make(distort, 4.0),
                    Style=skia.Paint.kStroke_Style,
                    StrokeWidth=3.0,
                    AntiAlias=True,
                    Color=0xFF4285F4)
                # canvas.clear(skia.ColorWHITE)
                path = CanvasDemo.star()
                canvas.drawPath(path, paint)
                draw_text(canvas)
                canvas.restore()
                canvas.translate(256, 256)
                paint = skia.Paint(AntiAlias=True)
                blob = skia.TextBlob('Skia', skia.Font(None, 36), [(0, 0), (32, 5), (64, -5), (96, 2)])
                canvas.drawTextBlob(blob, 10, 48, paint)
                xform = [
                    skia.RSXform(1, 0, 0, 0),
                    skia.RSXform(1.2, 0, 28, 0),
                    skia.RSXform(0.8, -0.1, 48, 0),
                    skia.RSXform(1, 0.2, 64, 0),
                ]
                blob = skia.TextBlob.MakeFromRSXform('Skia', xform, skia.Font(None, 36))
                canvas.drawTextBlob(blob, 10, 96, paint)

            distort += 0.01
            rotation += 1
            if distort >= 10:
                distort = 5


async def main():
    window = Window(title='Skia Surface')
    window.position = (50, 50)
    window.size = (1024, 768)
    surface_demo = CanvasDemo()
    asyncio.create_task(surface_demo.update())
    await window.serve(UserInterface(content=surface_demo))


asyncio.run(main())
