from p3ui import *
import asyncio
from imageio import imread
import skia
import numpy as np

repeat_x = 12
repeat_y = 12

minions = imread('minions.png')
minions = np.concatenate((minions, np.ones((minions.shape[0], minions.shape[1], 1)) * 255.), axis=2)
rgba = np.zeros((minions.shape[0] * repeat_y, minions.shape[1] * repeat_x, 4))
for x in range(repeat_x):
    for y in range(repeat_y):
        rgba[y * minions.shape[0]:(y + 1) * minions.shape[0], x * minions.shape[1]:(x + 1) * minions.shape[1],
        :] = minions
image = skia.Image.fromarray(rgba.astype(np.uint8), skia.ColorType.kRGBA_8888_ColorType)
print(rgba.shape[1])


class ImageViewer(Layout):

    def __init__(self):
        self._picture = Picture(width=(rgba.shape[1] | px, 1, 1), height=(rgba.shape[0] | px, 1, 1))
        super().__init__(
            direction=Direction.Vertical,
            padding=(0 | px, 0 | px),
            children=[
                Button(label='b1'),
                self._picture,
                Button(label='b1')
            ]
        )

        with self._picture as canvas:
            canvas.save()
            canvas.drawImage(image, 0, 0)


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
    user_interface = UserInterface(content=ScrollArea(content=iw))
    user_interface.theme.make_light()
    await window.serve(user_interface)


if __name__ == "__main__":
    asyncio.run(main())
