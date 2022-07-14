#include "p3ui.h"

#include <p3/widgets/MenuItem.h>

namespace p3::python {

void ArgumentParser<MenuItem>::operator()(py::kwargs const& kwargs, MenuItem& menu_item)
{
    ArgumentParser<Node>()(kwargs, menu_item);
    assign(kwargs, "enabled", menu_item, &MenuItem::set_enabled);
    assign(kwargs, "shortcut", menu_item, &MenuItem::set_shortcut);
    assign(kwargs, "checkable", menu_item, &MenuItem::set_checkable);
    assign(kwargs, "value", menu_item, &MenuItem::set_value);
    assign(kwargs, "enabled", menu_item, &MenuItem::set_enabled);
    assign(kwargs, "on_click", menu_item, &MenuItem::set_on_click);
    assign(kwargs, "on_change", menu_item, &MenuItem::set_on_change);
}

void Definition<MenuItem>::apply(py::module& module)
{
    py::class_<MenuItem, Node, std::shared_ptr<MenuItem>> menu_item(module, "MenuItem");
    menu_item.def(py::init<>([](std::string label, py::kwargs kwargs) {
        auto menu = std::make_shared<MenuItem>(std::move(label));
        ArgumentParser<MenuItem>()(kwargs, *menu);
        return menu;
    }));

    def_property(menu_item, "checkable", &MenuItem::checkable, &MenuItem::set_checkable);
    def_property(menu_item, "value", &MenuItem::value, &MenuItem::set_value);
    def_property(menu_item, "enabled", &MenuItem::enabled, &MenuItem::set_enabled);
    def_signal_property(menu_item, "on_click", &MenuItem::on_click, &MenuItem::set_on_click);
    def_signal_property(menu_item, "on_change", &MenuItem::on_change, &MenuItem::set_on_change);
}

}
