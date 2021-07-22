/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#include "p3ui.h"
#include <p3/MenuItem.h>

namespace p3::python
{

    void Definition<MenuItem>::parse(py::kwargs const& kwargs, MenuItem& menu_item)
    {
        Definition<Node>::parse(kwargs, menu_item);
        if (kwargs.contains("enabled"))
            menu_item.set_enabled(kwargs["enabled"].cast<bool>());
        if (kwargs.contains("shortcut"))
            menu_item.set_shortcut(kwargs["shortcut"].cast<std::optional<std::string>>());
        if (kwargs.contains("checkable"))
            menu_item.set_checkable(kwargs["checkable"].cast<bool>());
        if (kwargs.contains("checked"))
            menu_item.set_checked(kwargs["checked"].cast<bool>());
        if (kwargs.contains("on_click"))
            menu_item.set_on_click([f = kwargs["on_click"].cast<py::function>()]() {
                py::gil_scoped_acquire acquire;
                f();
            });
        if (kwargs.contains("on_change"))
            menu_item.set_on_change([f = kwargs["on_change"].cast<py::function>()](bool checked) {
                py::gil_scoped_acquire acquire;
                f(checked);
            });
    }

    void Definition<MenuItem>::apply(py::module& module)
    {
        py::class_<MenuItem, Node, std::shared_ptr<MenuItem>> menu_item(module, "MenuItem");
        menu_item.def(py::init<>([](std::string label, py::kwargs kwargs) {
            auto menu = std::make_shared<MenuItem>(std::move(label));
            parse(kwargs, *menu);
            return menu;
        }));
        menu_item.def_property("checkable", &MenuItem::checkable, &MenuItem::set_checkable);
        menu_item.def_property("checked", &MenuItem::checked, &MenuItem::set_checked);
        menu_item.def_property("enabled", &MenuItem::enabled, &MenuItem::set_enabled);
        menu_item.def_property("on_click", &MenuItem::on_click, [](MenuItem& menu_item, py::function f) {
            menu_item.set_on_click([f{std::move(f)}]() {
                py::gil_scoped_acquire acquire;
                f();
            });
        });
        menu_item.def_property("on_change", &MenuItem::on_change, [](MenuItem& menu_item, py::function f) {
            menu_item.set_on_change([f{std::move(f)}](bool checked) {
                py::gil_scoped_acquire acquire;
                f(checked);
            });
        });
    }

}
