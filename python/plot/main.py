from p3ui import *
import asyncio

import matplotlib

matplotlib.use("module://p3.matplotlib.backend")


async def main():
    window = Window(title='Canvas Demo')
    window.position = (50, 50)
    window.size = (800, 640)
    await window.serve(UserInterface(content=None))


asyncio.run(main())
