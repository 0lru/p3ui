from p3ui.native import EventLoop
import asyncio
import concurrent
import traceback
import logging

logger = logging.getLogger('p3ui')


class GuiEventLoop(asyncio.AbstractEventLoop):

    def __init__(self, *args, **kwargs):
        self._running = False
        super().__init__(*args, **kwargs)
        self.__native_event_loop = EventLoop()
        self._default_executor = None

        self._current_handle = None

    def get_debug(self):
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

    def call_soon(self, callback, *args, context=None):
        handle = asyncio.Handle(callback, args, self, context)
        self.__native_event_loop.call_at(0, handle)
        return handle

    def call_soon_threadsafe(self, callback, *args, context=None):
        handle = asyncio.Handle(callback, args, self, context)
        self.__native_event_loop.call_at(0, handle)
        return handle

    def call_later(self, delay, callback, *args, context=None):
        #
        # TODO: use more decent time representation
        return self.call_at(self.time + delay, callback, *args, context=None)

    def call_at(self, when, callback, *args, context=None):
        h = asyncio.TimerHandle(when, callback, args, self, context)
        self.__native_event_loop.call_at(when, h)
        h._scheduled = True
        return h

    def create_task(self, coro):
        return asyncio.Task(coro, loop=self)

    def create_future(self):
        return asyncio.Future(loop=self)

    def run_in_executor(self, executor, func, *args, **kwargs):
        if executor is None:
            executor = self._default_executor
            if executor is None:
                executor = concurrent.futures.ThreadPoolExecutor(thread_name_prefix='p3ui')
                self._default_executor = executor
        return asyncio.futures.wrap_future(executor.submit(func, *args), loop=self)

    def shutdown_default_executor(self):
        if self._default_executor is None:
            return
        self._default_executor.shutdown(wait=True)

    def call_exception_handler(self, context):
        """
        slightly modified implementation from asyncio.
        """
        message = context.get('message')
        if not message:
            message = 'Unhandled exception in event loop'

        exception = context.get('exception')
        if exception is not None:
            exc_info = (type(exception), exception, exception.__traceback__)
        else:
            exc_info = False

        if 'source_traceback' not in context:
            if self._current_handle is not None:
                if self._current_handle._source_traceback is not None:
                    context['handle_traceback'] = self._current_handle._source_traceback

        log_lines = [message]
        for key in sorted(context):
            if key in {'message', 'exception'}:
                continue
            value = context[key]
            if key == 'source_traceback':
                tb = ''.join(traceback.format_list(value))
                value = 'Object created at (most recent call last):\n'
                value += tb.rstrip()
            elif key == 'handle_traceback':
                tb = ''.join(traceback.format_list(value))
                value = 'Handle created at (most recent call last):\n'
                value += tb.rstrip()
            else:
                value = repr(value)
            log_lines.append(f'{key}: {value}')

        logger.error('\n'.join(log_lines), exc_info=exc_info)
