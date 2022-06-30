#include "Promise.h"
#include "p3ui.h"

#include <p3/Theme.h>
#include <p3/UserInterface.h>

#include <p3/platform/Window.h>

#include <p3/widgets/MenuBar.h>
#include <p3/widgets/Popup.h>

namespace p3::python {

void Definition<Window>::apply(py::module& module)
{
    py::class_<Timer, std::shared_ptr<Timer>>(module, "Timer")
        .def(py::init<>([]() { return std::make_shared<Timer>(); }))
        .def("time", [](Timer& timer) { return std::chrono::duration_cast<std::chrono::duration<double>>(timer.time()).count(); })
        .def("time_and_reset", [](Timer& timer) { return std::chrono::duration_cast<std::chrono::duration<double>>(timer.time_and_reset()).count(); });

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
        .def(py::init<>([](int x, int y) { return Window::Position { x, y }; }))
        .def(py::init<>([](std::tuple<int, int> size) { return Window::Position { std::get<0>(size), std::get<1>(size) }; }))
        .def_readwrite("x", &Window::Position::x)
        .def_readwrite("y", &Window::Position::y);
    py::implicitly_convertible<py::tuple, Window::Position>();

    py::class_<Window::Size>(window, "Size")
        .def(py::init<>([](int width, int height) { return Window::Size { width, height }; }))
        .def(py::init<>([](std::tuple<int, int> size) { return Window::Size { std::get<0>(size), std::get<1>(size) }; }))
        .def_readwrite("width", &Window::Size::width)
        .def_readwrite("height", &Window::Size::height);
    py::implicitly_convertible<py::tuple, Window::Size>();

    window.def(py::init<>([](std::string title, std::size_t width, std::size_t height, py::kwargs kwargs) {
        auto window = std::make_shared<Window>(std::move(title), width, height);

        auto window_dict = std::make_shared<py::dict>();
        (*window_dict)["children"] = py::list();
        window->set_user_data(std::make_shared<py::dict>());

        auto user_interface_dict = std::make_shared<py::dict>();
        (*user_interface_dict)["children"] = py::list();
        window->user_interface()->set_user_data(std::make_shared<py::dict>());

        (*std::static_pointer_cast<py::dict>(window->user_data()))["content"] = py::cast(window->user_interface());

        //
        // release the gil while rendering
        window->set_render_scope([](std::function<void()> render) {
            py::gil_scoped_release release;
            render();
        });

        return window;
    }),
        py::kw_only(),
        py::arg("title") = "p3",
        py::arg("width") = 1024,
        py::arg("height") = 768);

    def_content_property(window, "user_interface", &Window::user_interface, &Window::set_user_interface);
    window.def_property_readonly("monitor", &Window::monitor);
    window.def_property_readonly("primary_monitor", &Window::primary_monitor);
    window.def_property_readonly("framebuffer_size", &Window::framebuffer_size);
    window.def_property_readonly("monitors", &Window::monitors);
    window.def_property_readonly("frames_per_second", &Window::frames_per_second);
    window.def_property_readonly("idle_timer", &Window::time_till_enter_idle_mode);
    window.def_property("video_mode", &Window::video_mode, &Window::set_video_mode);
    window.def_property("position", &Window::position, &Window::set_position);
    window.def_property("size", &Window::size, &Window::set_size);
    window.def_property("vsync", &Window::vsync, &Window::set_vsync);
    window.def_property("idle_timeout", &Window::idle_timeout, &Window::set_idle_timeout);
    window.def_property("idle_frame_time", &Window::idle_frame_time, &Window::set_idle_frame_time);
    window.def_property("user_interface", &Window::user_interface, &Window::set_user_interface);

    window.def_property_readonly("closed", [&](Window& w) {
        auto asyncio = py::module::import("asyncio");
        auto promise_impl = std::make_unique<Promise<void>>(asyncio);
        auto future = promise_impl->get_future();
        w.set_close_callback([promise { p3::Promise<void>(std::move(promise_impl)) }]() mutable {
            promise.set_value();
        });
        return future;
    });
}

}
