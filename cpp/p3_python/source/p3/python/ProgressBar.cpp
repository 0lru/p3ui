#include "p3ui.h"

#include <p3/widgets/ProgressBar.h>


namespace p3::python
{

    void Definition<ProgressBar>::apply(py::module& module)
    {
        py::class_<ProgressBar, Node, std::shared_ptr<ProgressBar>> progress_bar(module, "ProgressBar");
        progress_bar.def(py::init<>([](py::kwargs kwargs) {
            auto progress_bar = std::make_shared<ProgressBar>();
            ArgumentParser<Node>()(kwargs, *progress_bar);
            assign(kwargs, "value", *progress_bar, &ProgressBar::set_value);
            return progress_bar;
        }));
        def_property(progress_bar, "value", &ProgressBar::value, &ProgressBar::set_value);
    }

}