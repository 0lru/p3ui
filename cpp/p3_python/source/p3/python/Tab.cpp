
#include "p3ui.h"
#include <p3/Tab.h>


namespace p3::python
{

    void Definition<Tab>::apply(py::module& module)
    {
        py::class_<Tab, Node, std::shared_ptr<Tab>> tab(module, "Tab");

        tab.def(py::init<>([](py::kwargs kwargs) {
            auto tab = std::make_shared<Tab>();
            ArgumentParser<Node>()(kwargs, *tab);
            return tab;
        }));

        auto tab_item = py::class_<Tab::Item, Node, std::shared_ptr<Tab::Item>>(module, "TabItem");

        tab_item.def(py::init<>([](std::string name, py::kwargs kwargs) {
            auto tab_item = std::make_shared<Tab::Item>(std::move(name));
            assign(kwargs, "content", *tab_item, &Tab::Item::set_content);
            return tab_item;
        }));
    }

}
