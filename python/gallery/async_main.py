import time

import p3ui as p3
import asyncio

user_interface = p3.UserInterface(content=p3.Text('Hello World!'))
window = p3.Window(user_interface=user_interface)


async def per_second():
    while True:
        await asyncio.sleep(1)


async def main():
    asyncio.get_event_loop().create_task(per_second())
    while not window.closed:
        window.frame()
        await asyncio.sleep(0)
    print('shutting down')


asyncio.run(main())
