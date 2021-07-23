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
#include <p3/CheckBox.h>

namespace p3::python
{

    void Definition<CheckBox>::parse(py::kwargs const& kwargs, CheckBox& check_box)
    {
        Definition<Node>::parse(kwargs, check_box);
        if (kwargs.contains("on_change"))
        {
            check_box.set_on_change([f{ kwargs["on_change"].cast<py::function>() }](bool value)
            {
                py::gil_scoped_acquire acquire;
                f(value);
            });
        }
        if (kwargs.contains("value"))
            check_box.set_value(kwargs["value"].cast<bool>());
    }

    void Definition<CheckBox>::apply(py::module& module)
    {
        py::class_<CheckBox, Node, std::shared_ptr<CheckBox>> check_box(module, "CheckBox");

        check_box.def(py::init<>([](std::optional<bool> value, py::kwargs kwargs) {
            auto check_box = std::make_shared<CheckBox>();
            parse(kwargs, *check_box);
            if (value)
                check_box->set_value(value.value());
            return check_box;
        }), py::kw_only(), py::arg("value")=py::none());

        check_box.def_property("on_change", &CheckBox::on_change, [](CheckBox& check_box, py::function f) {
            check_box.set_on_change([f{ std::move(f) }](bool value) {
                py::gil_scoped_acquire acquire;
                f(value);
            });
        });

        check_box.def_property("value", &CheckBox::value, &CheckBox::set_value);
    }

}