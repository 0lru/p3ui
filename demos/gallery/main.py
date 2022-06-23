import asyncio
from p3ui import *
from gallery import Gallery
import pathlib


async def main():
    window = Window(title='gallery')
    window.position = (50, 50)
    window.size = (1000, 900)
    gallery = Gallery(window)
    window.user_interface = gallery
    await window.closed


#    await gallery.shutdown()


run(main())
