from p3ui import *
import asyncio
from imageio import imread
import p3ui.skia as skia
import numpy as np

repeat_x = 100
repeat_y = 10

base = imread('image.png')
# minions = np.concatenate((minions, np.ones((minions.shape[0], minions.shape[1], 1)) * 255.), axis=2)
rgba = np.zeros((base.shape[0] * repeat_y, base.shape[1] * repeat_x, 4))
for x in range(repeat_x):
    for y in range(repeat_y):
        rgba[y * base.shape[0]:(y + 1) * base.shape[0], x * base.shape[1]:(x + 1) * base.shape[1],
        :] = base
image = skia.Image.fromarray(rgba.astype(np.uint8), skia.ColorType.kRGBA_8888_ColorType)
print(f'image={rgba.shape[1]}x{rgba.shape[0]}x{rgba.shape[2]}')


class ImageViewer(Layout):

    def __init__(self):
        self._surface = Surface(
            width=(rgba.shape[1] | px, 1, 1),
            height=(rgba.shape[0] | px, 1, 0)
        )
        super().__init__(
            direction=Direction.Vertical,
            justify_content=Justification.Center,
            align_items=Alignment.Stretch,
            children=[Button(label=f'button {i}') for i in range(10)] + [
                self._surface,
            ] + [Button(label=f'button {i}') for i in range(10)]
        )

        #with self._picture as canvas:
        #    canvas.save()
        #    canvas.drawImage(image, 0, 0)

    @staticmethod
    def star():
        """
        @see https://github.com/kyamagu/skia-python/blob/main/notebooks/Showcase.ipynb
        """
        from math import cos, sin
        radius = 256.0
        center = 256.0
        path = skia.Path()
        path.moveTo(center + radius, center)
        for i in range(1, 8):
            a = 2.6927937 * i
            path.lineTo(center + radius * cos(a), center + radius * sin(a))
        return path

    async def update(self):
        try:
            rotation = 0
            distort = 5
            while True:
                await asyncio.sleep(0.01)
                with self._surface as canvas:
                    canvas.drawImage(image, 0, 0)
                    canvas.rotate(rotation, 256, 256)
                    paint = skia.Paint(
                        PathEffect=skia.DiscretePathEffect.Make(distort, 4.0),
                        Style=skia.Paint.kStroke_Style,
                        StrokeWidth=10.0,
                        AntiAlias=True,
                        Color=0xFF4285F4)
                    path = ImageViewer.star()
                    canvas.drawPath(path, paint)
                distort += 0.01
                rotation += 1
                if distort >= 10:
                    distort = 5
        except asyncio.CancelledError:
            pass


async def main():
    window = Window(title='Large Image')
    window.size = (640, 480)
    window.position = (100, 100)

    iw = ImageViewer()
    scroll = ScrollArea(content=iw)
    window.user_interface.theme.make_dark()
    window.user_interface.content = scroll
    t = asyncio.create_task(iw.update())
    await window.closed
    t.cancel()


if __name__ == "__main__":
    run(main())
