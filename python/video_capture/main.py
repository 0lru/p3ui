import asyncio
import skia
from p3ui import *
import cv2

video = cv2.VideoCapture(0)


class Viewer(ScrollArea):

    def __init__(self):
        super().__init__()
        self.surface = Surface(1, 1)
        self.content = self.surface

    async def update(self):
        rotation = 0
        color = 0
        while True:
            _, frame = video.read()
            rgba = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)
            skia_rgba = skia.Image.fromarray(rgba, skia.ColorType.kRGBA_8888_ColorType)
            self.surface.height, self.surface.width = rgba.shape[0:2]
            with self.surface as canvas:
                canvas.save()
                canvas.rotate(rotation, rgba.shape[1] / 2, rgba.shape[0] / 2)
                rotation += 5
                canvas.drawImage(skia_rgba, 0, 0)
                canvas.restore()
                canvas.translate(100, 100)
                paint = skia.Paint(
                    Style=skia.Paint.kStroke_Style,
                    AntiAlias=True,
                    StrokeWidth=4,
                    Color=0xFF9900FF)
                rect = skia.Rect.MakeXYWH(10, 10, 100, 160)
                oval = skia.RRect()
                oval.setOval(rect)
                oval.offset(40, 80)
                canvas.drawRRect(oval, paint)
                canvas.drawRect(rect, paint)


async def main():
    window = Window(title='video')
    window.position = (256, 256)
    window.size = (512, 512)
    viewer = Viewer()
    asyncio.create_task(viewer.update())
    await window.serve(UserInterface(content=viewer))


asyncio.run(main())
