/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/
#pragma once

#include <p3/TaskQueue.h>

namespace p3::python
{

    template<typename T>
    class Promise
        : public p3::Promise<T>::Implementation
    {
    public:
        Promise(py::module_& asyncio)
            : _loop(asyncio.attr("get_event_loop")())
            , _future(_loop.attr("create_future")())
        {
        }

        py::object get_future() const
        {
            return _future;
        }

        py::object get_loop() const
        {
            return _loop;
        }

        void set_value(T t) override final
        {
            py::gil_scoped_acquire acquire;
            _loop.attr("call_soon_threadsafe")(_future.attr("set_result"), py::cast(std::move(t)));
            //
            // need to release that with gil..
            _loop.release();
            _future.release();
        }

        void set_exception(std::exception_ptr e) override final
        {
            py::gil_scoped_acquire acquire;
            _loop.attr("call_soon_threadsafe")(_future.attr("set_exception"), py::cast(std::move(e)));
            //
            // need to release that with gil..
            _loop.release();
            _future.release();
        }

    private:
        py::object _loop;
        py::object _future;
    };

    template<>
    class Promise<void>
        : public p3::Promise<void>::Implementation
    {
    public:
        Promise(py::module_& asyncio)
            : _loop(asyncio.attr("get_event_loop")())
            , _future(_loop.attr("create_future")())
        {
        }

        py::object get_future() const
        {
            return _future;
        }

        py::object get_loop() const
        {
            return _loop;
        }

        void set_value() override final
        {
            py::gil_scoped_acquire acquire;
            _loop.attr("call_soon_threadsafe")(_future.attr("set_result"), py::none());
            //
            // need to release that with gil..
            _loop.release();
            _future.release();
        }

        void set_exception(std::exception_ptr e) override final
        {
            py::gil_scoped_acquire acquire;
            _loop.attr("call_soon_threadsafe")(_future.attr("set_exception"), py::cast(std::move(e)));
            //
            // need to release that with gil..
            _loop.release();
            _future.release();
        }

    private:
        py::object _loop;
        py::object _future;
    };

}
