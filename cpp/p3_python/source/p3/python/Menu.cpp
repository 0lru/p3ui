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
#include <p3/Menu.h>

namespace p3::python
{

    void Definition<Menu>::parse(py::kwargs const& kwargs, Menu& menu)
    {
        Definition<Node>::parse(kwargs, menu);
        if (kwargs.contains("on_open"))
            menu.set_on_open([f = kwargs["on_open"].cast<py::function>()]() {
                py::gil_scoped_acquire acquire;
                f();
            });
        if (kwargs.contains("on_close"))
            menu.set_on_close([f = kwargs["on_close"].cast<py::function>()]() {
                py::gil_scoped_acquire acquire;
                f();
            });
    }

    void Definition<Menu>::apply(py::module& module)
    {
        py::class_<Menu, Node, std::shared_ptr<Menu>> menu(module, "Menu");
        menu.def(py::init<>([](std::string label, py::kwargs kwargs) {
            auto menu = std::make_shared<Menu>(label);
            parse(kwargs, *menu);
            return menu;
        }));
        menu.def("add", &Menu::add);
        menu.def("remove", &Menu::remove);
        menu.def_property("on_open", &Menu::on_open, [](Menu& menu, py::function f) {
            menu.set_on_open([f{std::move(f)}]() {
                py::gil_scoped_acquire acquire;
                f();
            });
        });
        menu.def_property("on_close", &Menu::on_close, [](Menu& menu, py::function f) {
            menu.set_on_close([f{std::move(f)}]() {
                py::gil_scoped_acquire acquire;
                f();
            });
        });
    }

}
