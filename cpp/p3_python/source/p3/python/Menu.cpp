#include "p3ui.h"

#include <p3/widgets/Menu.h>

namespace p3::python
{

    void Definition<Menu>::apply(py::module& module)
    {
        auto menu = py::class_<Menu, Node, std::shared_ptr<Menu>>(module, "Menu");

        menu.def(py::init<>([](std::string label, py::kwargs kwargs) {
            auto menu = std::make_shared<Menu>(label);
            ArgumentParser<Node>()(kwargs, *menu);
            assign(kwargs, "on_open", *menu, &Menu::set_on_open);
            assign(kwargs, "on_close", *menu, &Menu::set_on_close);
            return menu;
        }));

        def_method(menu, "add", &Menu::add);
        def_method(menu, "remove", &Menu::remove);
        def_property(menu, "on_open", &Menu::on_open, &Menu::set_on_open);
        def_property(menu, "on_close", &Menu::on_close, &Menu::set_on_close);
    }

}
