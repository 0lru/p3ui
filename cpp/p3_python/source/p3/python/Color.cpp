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

#include "p3ui.h"
#include <p3/Color.h>
#include <format>

namespace p3::python
{

    /*void Definition<Color>::parse(py::kwargs const& kwargs, Color& color)
    {
        Definition<Node>::parse(kwargs, color);
    }*/

    struct ColorTest
    {
        Color color;
    };

    template<typename T>
    std::uint8_t color_component(T& object)
    {
        return py::isinstance<std::uint8_t>(object)
            ? object.cast<std::uint8_t>()
            : std::uint8_t(object.cast<double>() * 255.);
    }

    void Definition<Color>::apply(py::module& module)
    {
        py::class_<Color> color(module, "Color");
        color.def(py::init<>());
        color.def(py::init<>([](py::tuple tuple) {
            return Color(
                static_cast<std::uint8_t>(py::isinstance<py::float_>(tuple[0]) 
                    ? tuple[0].cast<float>()*255.f 
                    : tuple[0].cast<std::uint8_t>()), 
                static_cast<std::uint8_t>(py::isinstance<py::float_>(tuple[1]) 
                    ? tuple[1].cast<float>()*255.f 
                    : tuple[1].cast<std::uint8_t>()), 
                static_cast<std::uint8_t>(py::isinstance<py::float_>(tuple[2]) 
                    ? tuple[2].cast<float>()*255.f 
                    : tuple[2].cast<std::uint8_t>()), 
                static_cast<std::uint8_t>(py::isinstance<py::float_>(tuple[3]) 
                    ? tuple[3].cast<float>()*255.f 
                    : tuple[3].cast<std::uint8_t>()));
        }));
        color.def(py::init<>([](py::list l) {
            return Color(
                static_cast<std::uint8_t>(py::isinstance<py::float_>(l[0]) 
                    ? l[0].cast<float>()*255.f 
                    : l[0].cast<std::uint8_t>()), 
                static_cast<std::uint8_t>(py::isinstance<py::float_>(l[1]) 
                    ? l[1].cast<float>()*255.f 
                    : l[1].cast<std::uint8_t>()), 
                static_cast<std::uint8_t>(py::isinstance<py::float_>(l[2]) 
                    ? l[2].cast<float>()*255.f 
                    : l[2].cast<std::uint8_t>()), 
                static_cast<std::uint8_t>(py::isinstance<py::float_>(l[3]) 
                    ? l[3].cast<float>()*255.f 
                    : l[3].cast<std::uint8_t>()));
        }));
        color.def(py::init<std::uint32_t>());
        color.def(py::init<std::string>());
        color.def(py::init<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>());
        color.def("__repr__", [](Color const& color) {
            return std::format("[{}, {}, {}, {}]", color.red(), color.green(), color.blue(), color.alpha());
        });

        py::class_<ColorTest> color_test(module, "ColorTest");
        color_test.def(py::init<>());
        color_test.def_readwrite("color", &ColorTest::color);

        // tuple -> color
        py::implicitly_convertible<py::tuple, Color>();
        py::implicitly_convertible<py::list, Color>();
        py::implicitly_convertible<std::string, Color>();

    }



}