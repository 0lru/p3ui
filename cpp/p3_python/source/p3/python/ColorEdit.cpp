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
#include <p3/ColorEdit.h>

namespace p3::python
{

    void Definition<ColorEdit>::apply(py::module& module)
    {
        py::class_<ColorEdit, Node, std::shared_ptr<ColorEdit>> color_edit(module, "ColorEdit");
        color_edit.def(py::init<>([](std::optional<py::function> on_change, std::optional<Color> value, py::kwargs kwargs) {
            auto color_edit = std::make_shared<ColorEdit>();
            ArgumentParser<Node>()(kwargs, *color_edit);
            assign(on_change, *color_edit, &ColorEdit::set_on_change);
            assign(value, *color_edit, &ColorEdit::set_value);
            return color_edit;
        }), py::kw_only(), py::arg("on_change")=py::none(), py::arg("value")=py::none());
        def_property(color_edit, "on_change", &ColorEdit::on_change, &ColorEdit::set_on_change);
        def_property(color_edit, "value", &ColorEdit::value, &ColorEdit::set_value);
    }

}