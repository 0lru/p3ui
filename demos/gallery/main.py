import asyncio
from p3ui import *
from gallery import Gallery


async def main():
    window = Window(title='gallery')
    window.position = (50, 50)
    window.size = (1000, 900)
    gallery = Gallery(window)
    await window.serve(gallery)
    await gallery.shutdown()


asyncio.run(main())
