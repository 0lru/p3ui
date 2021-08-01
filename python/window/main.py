import asyncio
from p3ui import *
import numpy as np

async def main():
    window = Window()
    print(window.position.x, window.position.y)
    button = Button(on_mouse_move=lambda e: print('moved'))
    for i in range(0, 500):
        window.position = Window.Position(550 - i, 550 - i)
        window.size = Window.Size(90 + i, i)
        print(window.position.x, window.position.y, window.size.width, window.size.height)


asyncio.run(main())
exit(0)

# width = window.monitor.mode.width
# height = window.monitor.mode.height


# async def main():
#    for i in range(0, 301, 2):
#        window.size = Window.Size(i + 340, i + 180)
#        window.position = Window.Position(i, i)
# await window.show(user_interface)


asyncio.run(main())
