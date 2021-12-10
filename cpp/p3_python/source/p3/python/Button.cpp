#include "p3ui.h"

#include <p3/widgets/Button.h>


namespace p3::python
{

    void Definition<Button>::apply(py::module& module)
    {
        py::class_<Button, Node, std::shared_ptr<Button>> button(module, "Button");
        button.def(py::init<>([](py::kwargs kwargs) {
            auto button = std::make_shared<Button>();
            ArgumentParser<Node>()(kwargs, *button);
            assign(kwargs, "on_click", *button, &Button::set_on_click);
            return button;
        }));
        def_property(button, "on_click", &Button::on_click, &Button::set_on_click);
    }

}
