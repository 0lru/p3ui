#include "p3ui.h"

#include <p3/platform/event_loop.h>

namespace p3::python {

class PythonTask : public Event {
public:
    PythonTask(py::object task)
        : _task(std::move(task))
    {
    }

    ~PythonTask() override
    {
        py::gil_scoped_acquire acquire;
        _task = py::none();
    }

    void operator()() override
    {
        py::gil_scoped_acquire acquire;
        _task.attr("_run")();
        _task = py::none();
    }

private:
    py::object _task;
};

void Definition<EventLoop>::apply(py::module& module)
{
    py::class_<EventLoop, std::shared_ptr<EventLoop>> event_loop(module, "EventLoop");

    event_loop.def(py::init<>([]() {
        return std::make_shared<EventLoop>();
    }));

    event_loop.def("run_forever", [](EventLoop& event_loop) {
        py::gil_scoped_release release;
        event_loop.run_forever();
    });
    event_loop.def("push", [](EventLoop& event_loop, double delay, py::object handle) {
        auto now = EventLoop::Clock::now();
        auto d = std::chrono::duration<double>(delay);
        auto task = std::make_unique<PythonTask>(std::move(handle));
        auto tp = now + std::chrono::duration_cast<std::chrono::nanoseconds>(d);
        event_loop.call_at(tp, std::move(task));
    });
    event_loop.def("time", [](EventLoop& event_loop) {
        return 0.0;
    });
    event_loop.def("stop", &EventLoop::stop);
    event_loop.def("close", &EventLoop::close);
    //    def_property(image, "texture", &Image::texture, &Image::set_texture);
    //    def_property(image, "scale", &Image::scale, &Image::set_scale);
    //    def_property(image, "on_click", &Image::on_click, &Image::set_on_click);
}

}
