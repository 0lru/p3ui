import asyncio
from .gui_event_loop import GuiEventLoop


async def __entry_task(loop, entry_function):
    e = None
    try:
        await entry_function
    except asyncio.CancelledError:
        pass
    except Exception as _:
        e = _
    finally:
        loop.stop()
    return e


def run(entry_function):
    loop = GuiEventLoop()
    asyncio.set_event_loop(loop)
    asyncio._set_running_loop(loop)
    task = loop.create_task(__entry_task(loop, entry_function))
    loop.run_forever()
    loop.shutdown_default_executor()
    loop.close()
    result = task.result()
    if result is not None:
        raise result
