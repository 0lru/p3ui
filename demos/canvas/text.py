from p3ui import *
import asyncio

family_names = [skia.FontMgr.RefDefault().getFamilyName(i) for i in range(skia.FontMgr.RefDefault().countFamilies())]


class TextSurface(Surface):

    def __init__(self):
        super().__init__(
            width=(100 | percent, 0, 0),
            height=(len(family_names) * 50.0 + 50. | px, 0, 0)
        )
        offset = 50
        with self as canvas:
            for name in family_names:
                typeface = skia.Typeface(name)
                font = skia.Font(typeface, 50.0, 1.0, 0.0)
                paint = skia.Paint(AntiAlias=True, Color=skia.ColorWHITE)
                canvas.drawString(f'{name}: ABC abc Xx 123', 0, offset, font, paint)
                offset += 50


async def main():
    window = Window(title='Skia Fonts')
    window.position = (50, 50)
    window.size = (1024, 768)
    await window.serve(UserInterface(content=ScrollArea(content=TextSurface())))


asyncio.run(main())
