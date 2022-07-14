#include "p3ui.h"

#include <p3/Surface.h>

#include <memory>
#include <string>

//
// obligatory for casting to sk_sp
PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

namespace p3::python {

class Surface : public p3::Surface {
public:
    py::object enter();
    void exit(py::args);

protected:
    std::optional<py::object> _recorder;
};

py::object Surface::enter()
{
    auto skia = py::module::import("p3ui.skia");
    // auto inf = skia.attr("SK_ScalarInfinity");
    auto inf = std::numeric_limits<float>::max();
    _recorder = skia.attr("PictureRecorder")();
    return _recorder.value().attr("beginRecording")(inf, inf);
}

void Surface::exit(py::args)
{
    // need to finalize recording with gil held
    std::optional<py::object> recording = _recorder.value().attr("finishRecordingAsPicture")();
    _recorder.reset();
    {
        //
        // PYBIND11_DECLARE_HOLDER_TYPE for sk_sp is obligatory
        auto picture = recording.value().cast<sk_sp<SkPicture>>();
        set_picture(std::move(picture));
    }
    // need to reset with gil
    recording.reset();
}

void Definition<Surface>::apply(py::module& module)
{
    py::class_<Surface, p3::Node, std::shared_ptr<Surface>> surface(module, "Surface");

    surface.def(py::init<>([](py::kwargs kwargs) {
        auto surface = std::make_shared<Surface>();
        ArgumentParser<p3::Node>()(kwargs, *surface);
        assign(kwargs, "on_click", static_cast<p3::Surface&>(*surface), &p3::Surface::set_on_click);
        assign(kwargs, "on_viewport_change", static_cast<p3::Surface&>(*surface), &p3::Surface::set_on_viewport_change);
        return surface;
    }));

    def_signal_property(surface, "on_click", &Surface::on_click, &Surface::set_on_click);
    def_signal_property(surface, "on_viewport", &Surface::on_viewport_change, &Surface::set_on_viewport_change);
    def_property_readonly(surface, "viewport", &Surface::viewport /*, &Surface::set_on_viewport*/);
    def_property_readonly(surface, "picture", &Surface::picture);
    surface.def("__enter__", &Surface::enter);
    surface.def("__exit__", &Surface::exit);
}

}
