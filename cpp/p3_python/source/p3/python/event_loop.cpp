#include "p3ui.h"

#include <p3/event_loop.h>

namespace p3::python {

class EventLoop : public p3::EventLoop {

protected:
    void process_work(Work work) override
    {
        py::gil_scoped_acquire acquire;
        p3::EventLoop::process_work(std::move(work));
    }
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
        auto d = std::chrono::duration < double>(delay);
        auto tp = now + std::chrono::duration_cast < std::chrono::nanoseconds>(d);
        event_loop.call_at(tp, [handle { std::move(handle) }]() {
            handle.attr("_run")();
        });
    });
    event_loop.def("time", [](EventLoop& event_loop) {
        return 0.0;
    });
    //    def_property(image, "texture", &Image::texture, &Image::set_texture);
    //    def_property(image, "scale", &Image::scale, &Image::set_scale);
    //    def_property(image, "on_click", &Image::on_click, &Image::set_on_click);
}

}
