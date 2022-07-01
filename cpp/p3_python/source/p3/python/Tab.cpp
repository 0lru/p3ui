#include "p3ui.h"
#include <p3/widgets/Tab.h>

namespace p3::python {

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
        auto dict = std::static_pointer_cast<py::dict>(tab_item->user_data());
        tab_item->set_user_data(dict);
        (*dict)["children"] = py::list();
        if (kwargs.contains("content")) {
            (*dict)["content"] = kwargs["content"];
            assign(kwargs, "content", *tab_item, &Tab::Item::set_content);
        }
        return tab_item;
    }));
    def_content_property(tab_item, "content", &Tab::Item::content, &Tab::Item::set_content);
}

}
