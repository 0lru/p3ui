#pragma once

#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <p3/forward.h>
#include <p3/log.h>
namespace py = pybind11;

#include "Promise.h"

namespace p3::python {

class Builder;
class Surface;

template <typename T, typename Object>
void assign(py::kwargs const& kwargs, const char* name, Object& object, void (Object::*setter)(T))
{
    if (kwargs.contains(name))
        (object.*setter)(kwargs[name].cast<T>());
}

//
// bit hacky.. revise later
struct FunctionGuard {
    py::function f;
    FunctionGuard(py::function f)
        : f(std::move(f))
    {
    }

    ~FunctionGuard()
    {
        // py::gil_scoped_acquire acquire;
        f = py::function();
    }
};

template <typename... Args, typename Object>
void assign(py::kwargs const& kwargs, const char* name, Object& object, void (Object::*setter)(std::function<void(Args...)>))
{
    if (kwargs.contains(name)) {
        if (!object.user_data())
            object.set_user_data(std::make_shared<py::dict>());
        auto user_data = std::static_pointer_cast<py::dict>(object.user_data());
        (*user_data)[name] = kwargs[name].cast<py::object>();
        auto weak_ref = py::weakref(kwargs[name].cast<py::object>(), {});
        (object.*setter)([weak_ref](Args... args) mutable {
            // py::gil_scoped_acquire acquire;
//            py::object f = weak_ref();
//            if (f != py::none())
                weak_ref(std::move(args)...);
        });
    }
}

template <typename T, typename Object>
void assign(std::optional<T>& value, Object& object, void (Object::*setter)(T))
{
    if (value)
        (object.*setter)(value.value());
}

template <typename... Args, typename Object>
void assign(std::optional<py::function>& f, Object& object, void (Object::*setter)(std::function<void(Args...)>))
{
    if (f)
        (object.*setter)([f = f.value()](Args... args) {
            // py::gil_scoped_acquire acquire;
            f(std::move(args)...);
        });
}

template <typename Target, typename F>
void def_method(Target& target, char const* name, F&& f)
{
    target.def(name, std::forward<F>(f));
}

template <typename Target, typename Getter, typename Setter>
void def_property(Target& target, char const* name, Getter&& getter, Setter setter)
{
    target.def_property(name, std::forward<Getter>(getter), std::forward<Setter>(setter));
}

template <typename Target, typename Getter>
void def_property_readonly(Target& target, char const* name, Getter&& getter)
{
    target.def_property_readonly(name, std::forward<Getter>(getter));
}

template <typename T>
struct ArgumentParser {
    void operator()(py::kwargs const& kwargs, T& item);
};

template <typename T>
void parse_kwargs(py::kwargs const& kwargs, T& item)
{
    ArgumentParser<T>()(kwargs, item);
}

template <typename T>
class Definition {
public:
    static void apply(py::module&);
};

}
