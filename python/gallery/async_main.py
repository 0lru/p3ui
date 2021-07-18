import p3ui as p3
import asyncio

user_interface = p3.UserInterface(content=p3.Text('Hello World!'), vsync=False)
window = p3.Window(user_interface=user_interface)


async def main():
    while not window.closed:
        window.frame()
        await asyncio.sleep(0.0020)


asyncio.run(main())
