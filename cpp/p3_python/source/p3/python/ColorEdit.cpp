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

    void Definition<ColorEdit>::parse(py::kwargs const& kwargs, ColorEdit& color_edit)
    {
        Definition<Node>::parse(kwargs, color_edit);
        if (kwargs.contains("on_change"))
        {
            color_edit.set_on_change([f{ kwargs["on_change"].cast<py::function>() }](Color value)
            {
                py::gil_scoped_acquire acquire;
                f(std::move(value));
            });
        }    
    }

    void Definition<ColorEdit>::apply(py::module& module)
    {
        py::class_<ColorEdit, Node, std::shared_ptr<ColorEdit>> color_edit(module, "ColorEdit");
        color_edit.def(py::init<>([](py::kwargs kwargs) {
            auto color_edit = std::make_shared<ColorEdit>();
            parse(kwargs, *color_edit);
            return color_edit;
        }));
        color_edit.def_property("on_change", &ColorEdit::on_change, [](ColorEdit& color_edit, py::function f) {
            color_edit.set_on_change([f{ std::move(f) }](Color value) {
                py::gil_scoped_acquire acquire;
                f(std::move(value));
            });
        });
        color_edit.def_property("value", &ColorEdit::value, &ColorEdit::set_value);
    }

}