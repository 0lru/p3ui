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
