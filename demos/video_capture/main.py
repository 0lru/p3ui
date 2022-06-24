import asyncio
import cv2

from p3ui import *


class Viewer(Surface):

    def __init__(self):
        super().__init__()
        self.video = cv2.VideoCapture(0)

    def get_frame(self):
        return self.video.read()

    async def update(self):
        try:
            rotation = 0
            while True:
                await asyncio.sleep(0)
                loop = asyncio.get_event_loop()
                _, frame = await loop.run_in_executor(None, self.video.read)
                rgba = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)
                skia_rgba = skia.Image.fromarray(rgba, skia.ColorType.kRGBA_8888_ColorType)
                with self as canvas:
                    canvas.save()
                    canvas.rotate(rotation, rgba.shape[1] / 2, rgba.shape[0] / 2)
                    rotation += 0.5
                    canvas.drawImage(skia_rgba, 0, 0)
                    canvas.restore()
                    canvas.save()
                    canvas.translate(100, 100)
                    paint = skia.Paint(
                        Style=skia.Paint.kStroke_Style,
                        AntiAlias=True,
                        StrokeWidth=4,
                        Color=0xFF9900FF)
                    rect = skia.Rect.MakeXYWH(10, 10, 100, 160)
                    oval = skia.RRect()
                    oval.setOval(rect)
                    #                oval.offset(40, 80)
                    canvas.scale(5, 5)
                    canvas.drawRRect(oval, paint)
                    canvas.drawRect(rect, paint)
                    canvas.restore()
        except asyncio.CancelledError:
            pass


async def main():
    window = Window(title='video')
    window.position = (256, 256)
    window.size = (512, 512)
    viewer = Viewer()
    window.user_interface.content = viewer
    t = asyncio.create_task(viewer.update())
    await window.closed
    t.cancel()
    await t


#    t.cancel()

run(main())
