import asyncio

from p3ui import *
from pathlib import Path

assets = Path(__file__).parents[1].joinpath('assets')


def make_paint_from_color(color):
    paint = skia.Paint()
    paint.setStrokeWidth(1.0)
    paint.setColor(color)
    return paint


async def main():
    window = Window(title='Skia Surface')
    window.position = (50, 50)
    window.size = (512, 512)

    stream = skia.FILEStream.MakeFromFile(assets.joinpath('SVG_logo.svg').as_posix())
    dom = skia.SVGDOM.MakeFromStream(stream)
    dom.setContainerSize(skia.Size(512, 512))
    print(dom.containerSize())

    surface = Surface()
    with surface as canvas:
        canvas.clear(skia.ColorWHITE)
#        canvas.scale(10, 10)
        dom.render(canvas)
    await window.serve(UserInterface(content=surface))


asyncio.run(main())
