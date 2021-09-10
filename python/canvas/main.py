import asyncio
from p3ui import *
import skia


class CanvasDemo(ScrollArea):

    def __init__(self):
        super().__init__()
        self.surface = Surface(256, 256)
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
                canvas.rotate(rotation, 128, 128)
                paint = skia.Paint(
                    PathEffect=skia.DiscretePathEffect.Make(distort, 4.0),
                    Style=skia.Paint.kStroke_Style,
                    StrokeWidth=3.0,
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
    window.size = (280, 280)
    surface_demo = CanvasDemo()
    asyncio.create_task(surface_demo.update())
    await window.serve(UserInterface(content=surface_demo))


asyncio.run(main())
