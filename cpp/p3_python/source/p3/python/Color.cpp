
#include "p3ui.h"
#include <p3/Color.h>
#include <fmt/format.h>


namespace p3::python
{

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
            return fmt::format("[{}, {}, {}, {}]", color.red(), color.green(), color.blue(), color.alpha());
        });

        // tuple -> color
        py::implicitly_convertible<py::tuple, Color>();
        py::implicitly_convertible<py::list, Color>();
        py::implicitly_convertible<std::string, Color>();
    }



}