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

    void Definition<CheckBox>::apply(py::module& module)
    {
        py::class_<CheckBox, Node, std::shared_ptr<CheckBox>> check_box(module, "CheckBox");

        check_box.def(py::init<>([](std::optional<py::function> on_change, std::optional<bool> value, std::optional<bool> radio, py::kwargs kwargs) {
            auto check_box = std::make_shared<CheckBox>();
            ArgumentParser<Node>()(kwargs, *check_box);
            assign(on_change, *check_box, &CheckBox::set_on_change);
            assign(value, *check_box, &CheckBox::set_value);
            assign(radio, *check_box, &CheckBox::set_radio);
            return check_box;
        }), py::kw_only(), py::arg("on_change")=py::none(), py::arg("value")=py::none(), py::arg("radio")=py::none());

        def_property(check_box, "on_change", &CheckBox::on_change, &CheckBox::set_on_change);
        def_property(check_box, "value", &CheckBox::value, &CheckBox::set_value);
        def_property(check_box, "radio", &CheckBox::radio, &CheckBox::set_radio);
    }

}