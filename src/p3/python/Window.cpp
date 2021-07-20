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
#include <p3/Popup.h>
#include <p3/Window.h>
#include <p3/Theme.h>
#include <p3/UserInterface.h>
#include <p3/MenuBar.h>

namespace p3::python
{

    void Definition<Window>::parse(py::kwargs const& kwargs, Window& window)
    {
    }

    void Definition<Window>::apply(py::module& module)
    {
        py::class_<Timer, std::shared_ptr<Timer>>(module, "Timer")
            .def(py::init<>([]() {
            return std::make_shared<Timer>();
        }))
            .def("time", [](Timer& timer) {
            return std::chrono::duration_cast<std::chrono::duration<double>>(timer.time()).count();
        })
            .def("time_and_reset", [](Timer& timer) {
            return std::chrono::duration_cast<std::chrono::duration<double>>(timer.time_and_reset()).count();
        });

        py::class_<VideoMode>(module, "VideoMode")
            .def_property_readonly("width", &VideoMode::width)
            .def_property_readonly("height", &VideoMode::height)
            .def_property_readonly("hz", &VideoMode::hz);

        py::class_<Monitor>(module, "Monitor")
            .def_property_readonly("name", &Monitor::name)
            .def_property_readonly("mode", &Monitor::mode)
            .def_property_readonly("modes", &Monitor::modes)
            .def("__eq__", [](Monitor& monitor, Monitor& other) {
            return monitor == other;
        });

        py::class_<Window, std::shared_ptr<Window>> window(module, "Window");

        py::class_<Window::Position>(window, "Position")
            .def(py::init<>([](int x, int y) { return Window::Position{ .x = x, .y = y }; }))
            .def_readwrite("x", &Window::Position::x)
            .def_readwrite("y", &Window::Position::y);

        py::class_<Window::Size>(window, "Size")
            .def(py::init<>([](int width, int height) { return Window::Size{ .width = width, .height = height }; }))
            .def_readwrite("width", &Window::Size::width)
            .def_readwrite("height", &Window::Size::height);

        window
            .def(py::init<>([](
                std::string title,
                std::size_t width,
                std::size_t height,
                bool vsync,
                std::optional<double> idle_timeout,
                double idle_frame_time,
                py::kwargs kwargs) {
            auto window = std::make_shared<Window>(std::move(title), width, height);
            if (kwargs.contains("user_interface"))
                window->set_user_interface(kwargs["user_interface"].cast<std::shared_ptr<UserInterface>>());
            window->set_vsync(vsync);
            parse(kwargs, *window);
            return window;
        }),
                py::kw_only(),
            py::arg("title") = "p3",
            py::arg("width") = 1024,
            py::arg("height") = 768,
            py::arg("vsync") = true,
            py::arg("idle_timeout") = py::none(),
            py::arg("idle_frame_time") = 1.0)
            .def_property("user_interface", &Window::user_interface, &Window::set_user_interface)
            .def_static("monitors", &Window::monitors)
            .def_property("position", &Window::position, &Window::set_position)
            .def_property("size", &Window::size, &Window::set_size)
            .def_property("vsync", &Window::vsync, &Window::set_vsync)
            .def_property("idle_timeout", [](Window& window) {
            return window.idle_timeout() ? std::optional<double>(window.idle_timeout().value().count()) : std::optional<double>();
        }, [](Window& window, std::optional<double> idle_timeout) {
            window.set_idle_timeout(idle_timeout
                ? std::optional<Window::Seconds>(idle_timeout.value())
                : std::nullopt);
        })
            .def_property("idle_frame_time", [](Window& window) {
            return window.idle_frame_time().count();
        }, [](Window& window, double idle_frame_time) {
            window.set_idle_frame_time(Window::Seconds(idle_frame_time));
        })
            .def_static("primary_monitor", &Window::primary_monitor)
            .def_property("video_mode", &Window::video_mode, &Window::set_video_mode)
            .def("frame", &Window::frame)
            .def_property_readonly("closed", &Window::closed)
            .def("loop", [](Window& window, py::object f) {
            Window::UpdateCallback on_frame;
            if (!f.is(py::none()))
                on_frame = [f{ f.cast<py::function>() }](auto window) {
                py::gil_scoped_acquire acquire;
                f(std::move(window));
            };
            py::gil_scoped_release release;
            window.loop(on_frame);
        }, py::kw_only(), py::arg("on_frame") = py::none());
    }

}
