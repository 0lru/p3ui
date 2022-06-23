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
    window = Window(title='SVG')
    window.position = (50, 50)
    window.size = (1000, 900)

    stream = skia.FILEStream.MakeFromFile(assets.joinpath('SVG_logo.svg').as_posix())
    dom = skia.SVGDOM.MakeFromStream(stream)
    dom.setContainerSize(skia.Size(900, 800))
    surface = Surface()
    with surface as canvas:
        canvas.clear(skia.ColorWHITE)
        canvas.translate(50, 50)
        dom.render(canvas)
    window.user_interface.content = surface
    await window.closed


run(main())
