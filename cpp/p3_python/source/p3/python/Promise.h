#pragma once

#include "p3ui.h"
#include <p3/Promise.h>

namespace p3::python {

template <typename T>
class Promise
    : public p3::Promise<T>::Implementation {
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
        _loop.attr("call_soon")(_future.attr("set_result"), py::cast(std::move(t)));
        //
        // need to release that with gil..
        _loop.release();
        _future.release();
    }

    void set_exception(std::exception_ptr e) override final
    {
        py::gil_scoped_acquire acquire;
        _loop.attr("call_soon")(_future.attr("set_exception"), py::cast(std::move(e)));
        //
        // need to release that with gil..
        _loop.release();
        _future.release();
    }

private:
    py::object _loop;
    py::object _future;
};

template <>
class Promise<void>
    : public p3::Promise<void>::Implementation {
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
        _loop.attr("call_soon")(_future.attr("set_result"), py::none());
        //
        // need to release that with gil..
        _loop.release();
        _future.release();
    }

    void set_exception(std::exception_ptr e) override final
    {
        py::gil_scoped_acquire acquire;
        _loop.attr("call_soon")(_future.attr("set_exception"), py::cast(std::move(e)));
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
