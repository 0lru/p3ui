
#include "p3ui.h"
#include "Promise.h"
#include "AsyncTaskQueue.h"

#include <p3/Popup.h>
#include <p3/platform/Window.h>
#include <p3/Theme.h>
#include <p3/UserInterface.h>
#include <p3/MenuBar.h>

namespace p3::python
{

    template<typename Object, typename Result, typename ... Args>
    auto gil_release(Result(Object::* member)(Args...) const)
    {
        return [member](Object& object, typename WrapParameter<Args>::ParameterType... param) -> Result
        {
            py::gil_scoped_release release;
            return (object.*member)(WrapParameter<Args>()(std::move(param))...);
        };
    }

    template<typename Object, typename Result, typename ... Args>
    auto gil_release(Result(Object::* member)(Args...))
    {
        return [member](Object& object, typename WrapParameter<Args>::ParameterType... param) -> Result
        {
            py::gil_scoped_release release;
            return (object.*member)(WrapParameter<Args>()(std::move(param))...);
        };
    }

    void Definition<Window>::apply(py::module& module)
    {
        auto asyncio = py::module::import("asyncio");

        py::class_<Timer, std::shared_ptr<Timer>>(module, "Timer")
            .def(py::init<>([]() { return std::make_shared<Timer>(); }))
            .def("time", [](Timer& timer) { return std::chrono::duration_cast<std::chrono::duration<double>>(timer.time()).count(); })
            .def("time_and_reset", [](Timer& timer) {return std::chrono::duration_cast<std::chrono::duration<double>>(timer.time_and_reset()).count(); });

        py::class_<VideoMode>(module, "VideoMode")
            .def_property_readonly("width", &VideoMode::width)
            .def_property_readonly("height", &VideoMode::height)
            .def_property_readonly("hz", &VideoMode::hz)
            .def("__eq__", [](VideoMode& self, VideoMode& other) {
                return self == other;
            });

        py::class_<Monitor>(module, "Monitor")
            .def_property_readonly("name", &Monitor::name)
            .def_property_readonly("mode", &Monitor::mode)
            .def_property_readonly("modes", &Monitor::modes)
            .def_property_readonly("dpi", &Monitor::dpi)
            .def("__eq__", [](Monitor& monitor, Monitor& other) {
            return monitor == other;
        });

        auto window = py::class_<Window, std::shared_ptr<Window>>(module, "Window");

        py::class_<Window::Position>(window, "Position")
            .def(py::init<>([](int x, int y) { return Window::Position{ x, y }; }))
            .def(py::init<>([](std::tuple<int, int> size) { return Window::Position{ std::get<0>(size), std::get<1>(size) };}))
            .def_readwrite("x", &Window::Position::x)
            .def_readwrite("y", &Window::Position::y);
        py::implicitly_convertible<py::tuple, Window::Position>();

        py::class_<Window::Size>(window, "Size")
            .def(py::init<>([](int width, int height) { return Window::Size{ width, height }; }))
            .def(py::init<>([](std::tuple<int, int> size) { return Window::Size{ std::get<0>(size), std::get<1>(size) };}))
            .def_readwrite("width", &Window::Size::width)
            .def_readwrite("height", &Window::Size::height);
        py::implicitly_convertible<py::tuple, Window::Size>();

        window.def(py::init<>([](std::string title, std::size_t width, std::size_t height, py::kwargs kwargs) {
            auto window = std::make_shared<Window>(std::move(title), width, height);
            // window->set_vsync(vsync);
            return window;
        }), py::kw_only(), py::arg("title") = "p3", py::arg("width") = 1024, py::arg("height") = 768);

        window.def_property_readonly("monitor", gil_release(&Window::monitor));
        window.def_property_readonly("primary_monitor", gil_release(&Window::primary_monitor));
        window.def_property_readonly("framebuffer_size", gil_release(&Window::framebuffer_size));
        window.def_property_readonly("monitors", gil_release(&Window::monitors));
        window.def_property_readonly("user_interface", gil_release(&Window::user_interface));
        window.def_property_readonly("frames_per_second", gil_release(&Window::frames_per_second));
        window.def_property("video_mode", gil_release(&Window::video_mode), gil_release(&Window::set_video_mode));
        window.def_property("position", gil_release(&Window::position), gil_release(&Window::set_position));
        window.def_property("size", gil_release(&Window::size), gil_release(&Window::set_size));
        window.def_property("vsync", gil_release(&Window::vsync), gil_release(&Window::set_vsync));
        window.def_property_readonly("idle_timer", gil_release(&Window::time_till_enter_idle_mode));
        window.def_property("idle_timeout", gil_release(&Window::idle_timeout), gil_release(&Window::set_idle_timeout));
        window.def_property("idle_frame_time", gil_release(&Window::idle_frame_time), gil_release(&Window::set_idle_frame_time));

        window.def("serve", [=](Window& window, std::shared_ptr<UserInterface> user_interface, py::object loop) mutable {
            auto promise_impl = std::make_unique<Promise<void>>(asyncio);
            auto future = promise_impl->get_future();
            auto promise_loop = promise_impl->get_loop();
            auto promise = p3::Promise<void>(std::move(promise_impl));
            if (loop.is(py::none()))
                loop = promise_loop;
            window.serve(
                std::move(promise),
                std::move(user_interface),
                std::make_shared<AsyncTaskQueue>(loop));
            return future;
        }, py::arg("user_interface"), py::arg("loop") = py::none());
    }

}
