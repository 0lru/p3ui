import asyncio
from .gui_event_loop import GuiEventLoop


async def __entry_task(loop, entry_function):
    await entry_function
    loop.stop()


def run(entry_function):
    loop = GuiEventLoop()
    asyncio.set_event_loop(loop)
    asyncio._set_running_loop(loop)
    loop.create_task(__entry_task(loop, entry_function))
    loop.run_forever()
    loop.shutdown_default_executor()
    loop.close()
