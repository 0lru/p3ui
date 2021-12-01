from p3ui import *
import asyncio
from imageio import imread
import skia
import numpy as np

repeat_x = 20
repeat_y = 2

minions = imread('minions.png')
minions = np.concatenate((minions, np.ones((minions.shape[0], minions.shape[1], 1)) * 255.), axis=2)
rgba = np.zeros((minions.shape[0] * repeat_y, minions.shape[1] * repeat_x, 4))
for x in range(repeat_x):
    for y in range(repeat_y):
        rgba[y * minions.shape[0]:(y + 1) * minions.shape[0], x * minions.shape[1]:(x + 1) * minions.shape[1],
        :] = minions
image = skia.Image.fromarray(rgba.astype(np.uint8), skia.ColorType.kRGBA_8888_ColorType)
print(f'image={rgba.shape[1]}x{rgba.shape[0]}x{rgba.shape[2]}')


class ImageViewer(Layout):

    def __init__(self):
        self._picture = Surface(
            width=(rgba.shape[1] | px, 1, 1),
            height=(rgba.shape[0] | px, 1, 0)
        )
        super().__init__(
            direction=Direction.Vertical,
            justify_content=Justification.Center,
            align_items=Alignment.Stretch,
            children=[
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1'),
                self._picture,
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1'),
                Button(label='b1')
            ]
        )

        with self._picture as canvas:
            canvas.save()
            canvas.drawImage(image, 0, 0)

    async def update(self):
        rotation = 0.
        while True:
            with self._picture as canvas:
                canvas.drawImage(image, 0, 0)
                rotation += 10.0
                canvas.translate(400, 400)
                canvas.rotate(rotation)
                canvas.translate(-100, -100)
                paint = skia.Paint(
                    Style=skia.Paint.kStroke_Style,
                    AntiAlias=True,
                    StrokeWidth=4,
                    Color=0xFF9900FF)
                rect = skia.Rect.MakeXYWH(10, 10, 100, 160)
                oval = skia.RRect()
                oval.setOval(rect)
                canvas.drawRRect(oval, paint)
                canvas.drawRect(rect, paint)
            await asyncio.sleep(0.01)


async def main():
    #
    # setup window & user interface. the "user interface" is defined independently of
    # the system window and defines the theme and the font.
    window = Window(title='Playground')
    window.size = (640, 480)
    window.position = (100, 100)

    #
    # inject backend & window into the ui
    iw = ImageViewer()
    scroll = ScrollArea(content=iw)
    user_interface = UserInterface(content=scroll)
    user_interface.theme.make_light()

    t = asyncio.create_task(iw.update())

    await window.serve(user_interface)

    t.cancel()

if __name__ == "__main__":
    asyncio.run(main())
