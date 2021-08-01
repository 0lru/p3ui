#pragma once

#include "p3ui.h"


namespace p3::python
{

    template<typename T>
    py::capsule make_capsule(std::shared_ptr<T> ptr)
    {
        return py::capsule(new std::shared_ptr<T>(ptr), [](void* ptr) {
            delete reinterpret_cast<std::shared_ptr<T>*>(ptr);
        });
    }

    template<typename Guard, typename T>
    py::array_t<T> make_guarded_array_overlay(std::shared_ptr<Guard> guard, std::vector<T>& vector)
    {
        return py::array_t<T>({ vector.size() }, { sizeof(T) }, vector.data(), make_capsule(guard));
    }

    template<typename T>
    void copy(py::array_t<T> array, std::vector<T>& vector)
    {
        auto in = array.request();
        if (in.shape.size() != 1)
            throw std::invalid_argument("array has wrong shape");
        auto src = reinterpret_cast<T*>(in.ptr);
        vector.resize(in.shape[0]);
        std::copy(src, &src[in.shape[0]], vector.data());
    }

    template<typename T>
    void copy(std::optional<py::array_t<T>> array, std::optional<std::vector<T>>& vector)
    {
        if (!array)
        {
            vector.reset();
        }
        else
        {
            if (!vector)
                vector = std::vector<T>();
            copy(array.value(), vector.value());
        }
    }

    template<typename T>
    py::array_t<T> wrap(T* data, std::size_t height, std::size_t width, std::size_t depth)
    {
        return py::array_t<T>(
            py::array::ShapeContainer({ height, width, depth }),
            py::array::StridesContainer({ width * depth * sizeof(T), depth * sizeof(T), sizeof(T) }),
            data);
    }

}
