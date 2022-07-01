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
void assign(py::function f, char const* name, Object& object, void (Object::*setter)(std::function<void(Args...)>))
{
    if (!object.user_data())
        std::runtime_error("gc object not set");
    auto user_data = std::static_pointer_cast<py::dict>(object.user_data());
    (*user_data)[name] = f;
    //
    // remove weak_ref once working
    (object.*setter)([weak_ref = py::weakref(f)](Args... args) mutable {
        auto x = weak_ref();
        if (x != py::none())
            x(std::move(args)...);
    });
}

template <typename... Args, typename Object>
void assign(py::kwargs const& kwargs, const char* name, Object& object, void (Object::*setter)(std::function<void(Args...)>))
{
    if (!kwargs.contains(name))
        return;
    if (kwargs[name].is_none())
        (object.*setter)(nullptr);
    else
        assign(kwargs[name].cast<py::function>(), name, object, setter);
}

template <typename T, typename Object>
void assign(std::optional<T>& value, Object& object, void (Object::*setter)(T))
{
    if (value)
        (object.*setter)(value.value());
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

template <typename Target, typename Object, typename... Args>
void def_signal_property(
    Target& target,
    char const* name,
    std::function<void(Args...)> (Object::*getter)() const,
    void (Object::*setter)(std::function<void(Args...)>))
{
    target.def_property(
        name,
        [name = std::string(name)](Object& object) {
            auto user_data = std::static_pointer_cast<py::dict>(object.user_data());
            return py::object((*user_data)[name.c_str()]);
        },
        [setter, name = std::string(name)](Object& object, py::object f) {
            if (f.is_none()) {
                auto user_data = std::static_pointer_cast<py::dict>(object.user_data());
                (*user_data)[name.c_str()] = py::none();
                (object.*setter)(nullptr);
            } else {
                assign(py::cast<py::function>(f), name.c_str(), object, setter);
            }
        });
}

template <typename Target, typename Object, typename Content>
void def_content_property(
    Target& target,
    char const* name,
    Content (Object::*getter)() const,
    void (Object::*setter)(Content))
{
    target.def_property(
        name,
        [name = std::string(name)](Object& object) {
            auto user_data = std::static_pointer_cast<py::dict>(object.user_data());
            if (!user_data)
                log_fatal("no user data");
            return user_data->contains(name.c_str()) ? (*user_data)[name.c_str()] : py::object(py::none());
        },
        [setter, name = std::string(name)](Object& object, py::object content) {
            auto user_data = std::static_pointer_cast<py::dict>(object.user_data());
            if (!user_data)
                log_fatal("no user data");
            (*user_data)[name.c_str()] = content;
            (object.*setter)(content.is_none() ? Content() : py::cast<Content>(content));
        });
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
