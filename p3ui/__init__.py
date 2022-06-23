from asyncio import futures

from p3ui.native import *
from .matplotlib_surface import MatplotlibSurface
from .mpl import pixels_to_points, points_to_pixels, dpi
import p3ui.skia as skia
import concurrent.futures

import asyncio


class GuiEventLoop(asyncio.AbstractEventLoop):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.__native_event_loop = EventLoop()
        self._running = False
        self._default_executor = None

    def get_debug(self):
        """ leftover?! """
        return False

    @property
    def time(self):
        """ returns "0" for now """
        return self.__native_event_loop.time()

    def run_forever(self):
        self._running = True
        self.__native_event_loop.run_forever()

    def run_until_complete(self, future):
        raise NotImplementedError

    def _timer_handle_cancelled(self, handle):
        pass

    def is_running(self):
        return self._running

    def is_closed(self):
        return not self._running

    def stop(self):
        self.__native_event_loop.stop()
        self._running = False

    def close(self):
        self.__native_event_loop.close()
        self._running = False

    def shutdown_asyncgens(self):
        pass

    def call_exception_handler(self, context):
        pass
        # self._exc = context.get('exception', None)

    def call_soon(self, callback, *args, **kwargs):
        handle = asyncio.Handle(callback, args, self)
        self.__native_event_loop.push(0, handle)
        return handle

    def call_soon_threadsafe(self, callback, *args, **kwargs):
        return self.call_soon(callback, *args, **kwargs)

    def call_later(self, delay, callback, *args):
        #
        # TODO: use more decent time representation
        return self.call_at(self.time + delay, callback, *args)

    def call_at(self, when, callback, *args):
        h = asyncio.TimerHandle(when, callback, args, self)
        self.__native_event_loop.push(when, h)
        h._scheduled = True
        return h

    def create_task(self, coro):
        return asyncio.Task(coro, loop=self)

    def create_future(self):
        return asyncio.Future(loop=self)

    def run_in_executor(self, executor, func, *args):
        # if self.closed:
        #    raise RuntimeError('...')
        if executor is None:
            executor = self._default_executor
            if executor is None:
                executor = concurrent.futures.ThreadPoolExecutor(thread_name_prefix='p3ui')
                self._default_executor = executor
        return futures.wrap_future(executor.submit(func, *args), loop=self)

    def shutdown_default_executor(self):
        if self._default_executor is None:
            return
        self._default_executor.shutdown(wait=True)


async def __entry_task(loop, entry_function):
    await entry_function
    loop.close()


def run(entry_function):
    loop = GuiEventLoop()
    asyncio.set_event_loop(loop)
    asyncio._set_running_loop(loop)
    loop.create_task(__entry_task(loop, entry_function))
    loop.run_forever()
    loop.shutdown_default_executor()
