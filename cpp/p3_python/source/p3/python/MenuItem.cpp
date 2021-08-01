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

    void ArgumentParser<MenuItem>::operator()(py::kwargs const& kwargs, MenuItem& menu_item)
    {
        ArgumentParser<Node>()(kwargs, menu_item);
        assign(kwargs, "enabled", menu_item, &MenuItem::set_enabled);
        assign(kwargs, "shortcut", menu_item, &MenuItem::set_shortcut);
        assign(kwargs, "checkable", menu_item, &MenuItem::set_checkable);
        assign(kwargs, "value", menu_item, &MenuItem::set_value);
        assign(kwargs, "enabled", menu_item, &MenuItem::set_enabled);
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
        def_property(menu_item, "on_click", &MenuItem::on_click, &MenuItem::set_on_click);
        def_property(menu_item, "on_change", &MenuItem::on_change, &MenuItem::set_on_change);
    }

}
