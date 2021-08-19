import asyncio
from p3ui import *
from gallery import Gallery


async def main():
    window = Window(title='gallery')
    window.position = Window.Position(50, 50)
    window.size = Window.Size(1024, 768)
    await window.serve(Gallery(window))


asyncio.run(main())
