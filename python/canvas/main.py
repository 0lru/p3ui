import asyncio
from p3ui import *
import skia


class CanvasDemo(ScrollArea):

    def __init__(self):
        super().__init__()
        self.surface = Surface(1024, 768)
        self.content = self.surface

    @staticmethod
    def star():
        """
        @see https://github.com/kyamagu/skia-python/blob/main/notebooks/Showcase.ipynb
        """
        from math import cos, sin
        R = 115.2
        C = 0.0
        path = skia.Path()
        path.moveTo(C + R, C)
        for i in range(1, 8):
            a = 2.6927937 * i
            path.lineTo(C + R * cos(a), C + R * sin(a))
        return path

    async def update(self):
        rotation = 0
        distort = 5
        while True:
            await asyncio.sleep(0.01)
            with self.surface as canvas:
                canvas.translate(128, 128)
                canvas.rotate(rotation)
                paint = skia.Paint(
                    PathEffect=skia.DiscretePathEffect.Make(distort, 4.0),
                    Style=skia.Paint.kStroke_Style,
                    StrokeWidth=2.0,
                    AntiAlias=True,
                    Color=0xFF4285F4)
                canvas.clear(skia.ColorWHITE)
                path = CanvasDemo.star()
                canvas.drawPath(path, paint)
            distort += 0.01
            rotation += 1
            if distort >= 10:
                distort = 5


async def main():
    window = Window(title='Canvas Demo')
    window.position = (50, 50)
    window.size = (256, 256)
    surface_demo = CanvasDemo()
    asyncio.create_task(surface_demo.update())
    await window.serve(UserInterface(content=surface_demo))


asyncio.run(main())
