#include "p3ui.h"

#include <p3/widgets/button.h>

namespace p3::python {

void Definition<Button>::apply(py::module& module)
{
    py::class_<Button, Node, std::shared_ptr<Button>> button(module, "Button");
    button.def(py::init<>([](py::kwargs kwargs) {
        auto button = std::make_shared<Button>();
        ArgumentParser<Node>()(kwargs, *button);
        assign(kwargs, "on_click", *button, &Button::set_on_click);
        assign(kwargs, "background_color", *button, &Button::set_background_color);
        return button;
    }));
    def_signal_property(button, "on_click", &Button::on_click, &Button::set_on_click);
    button.def_property("background_color", &Button::background_color, &Button::set_background_color);
}

}
