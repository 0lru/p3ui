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

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>

#include <p3/log.h>
#include <p3/forward.h>
namespace py = pybind11;

#include "Promise.h"



namespace p3::python
{
    class Builder;

    //
    // wrap parameters
    // TODO: counterpart for result types
    template<typename T>
    struct WrapParameter
    {
        using ResultType = T;
        using ParameterType = T;
        ResultType operator()(T&& param) { return param; }
    };

    //
    // acquire gil if parameter is a callback
    template<typename ...Args>
    struct WrapParameter<std::function<void(Args...)>>
    {
        using ResultType = std::function<void(Args...)>;
        using ParameterType = py::function;
        ResultType operator()(ParameterType f)
        {
            return[f{ std::move(f) }](Args ... args) {
                py::gil_scoped_acquire acquire;
                f(std::move(args)...);
            };
        }
    };


    template<typename Synchronizable, typename Result, typename ... Args>
    auto sync(Result(Synchronizable::* member)(Args...))
    {
        return [member](Synchronizable& synchronizable, typename WrapParameter<Args>::ParameterType... param) -> Result
        {
            decltype(synchronizable.lock()) guard;
            {
                py::gil_scoped_release release;
                guard = synchronizable.lock();
            }
            return (synchronizable.*member)(WrapParameter<Args>()(std::move(param))...);
        };
    }

    template<typename Synchronizable, typename Result, typename ... Args>
    auto sync(Result(Synchronizable::* member)(Args...) const)
    {
        return [member](Synchronizable& synchronizable, typename WrapParameter<Args>::ParameterType... param) -> Result
        {
            decltype(synchronizable.lock()) guard;
            {
                py::gil_scoped_release release;
                guard = synchronizable.lock();
            }
            return (synchronizable.*member)(WrapParameter<Args>()(std::move(param))...);
        };
    }

    template<typename T, typename Object>
    void assign(py::kwargs const& kwargs, const char* name, Object& object, void (Object::* setter)(T))
    {
        if (kwargs.contains(name))
            (object.*setter)(kwargs[name].cast<T>());
    }

    template<typename ...Args, typename Object>
    void assign(py::kwargs const& kwargs, const char* name, Object& object, void (Object::* setter)(std::function<void(Args...)>))
    {
        if (kwargs.contains(name)) (object.*setter)([f{ kwargs[name].cast<py::function>() }](Args ... args) {
            py::gil_scoped_acquire acquire;
            f(std::move(args)...);
        });
    }

    template<typename T, typename Object>
    void assign(std::optional<T>& value, Object& object, void (Object::* setter)(T))
    {
        if (value) (object.*setter)(value.value());
    }

    template<typename ...Args, typename Object>
    void assign(std::optional<py::function>& f, Object& object, void (Object::* setter)(std::function<void(Args...)>))
    {
        if (f) (object.*setter)([f=f.value()](Args ... args) {
            py::gil_scoped_acquire acquire;
            f(std::move(args)...);
        });
    }

    template<typename Target, typename F>
    void def_method(Target& target, char const *name, F&& f)
    {
        target.def(name, sync(std::forward<F>(f)));
    }

    template<typename Target, typename Getter, typename Setter>
    void def_property(Target& target, char const *name, Getter&& getter, Setter setter)
    {
        target.def_property(name, sync(std::forward<Getter>(getter)), sync(std::forward<Setter>(setter)));
    }

    template<typename Target, typename Getter>
    void def_property_readonly(Target& target, char const *name, Getter&& getter)
    {
        target.def_property_readonly(name, sync(std::forward<Getter>(getter)));
    }

    template<typename T>
    struct ArgumentParser 
    {
        void operator()(py::kwargs const& kwargs, T& item);
    };

    template<typename T>
    void parse_kwargs(py::kwargs const& kwargs, T& item)
    {
        ArgumentParser<T>()(kwargs, item);
    }

    template<typename T>
    class Definition
    {
    public:
        static void apply(py::module&);
    };

}
