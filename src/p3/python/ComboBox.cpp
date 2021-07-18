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
#include <p3/ComboBox.h>


namespace p3::python
{

    void Definition<ComboBox>::parse(py::kwargs const& kwargs, ComboBox& combo)
    {
        Definition<Node>::parse(kwargs, combo);
        if (kwargs.contains("options"))
            combo.set_options(kwargs["options"].cast<std::vector<std::string>>());
        if (kwargs.contains("selected_index"))
            combo.set_selected_index(kwargs["selected_index"].cast<int>());
        if (kwargs.contains("on_change"))
        {
            combo.set_on_change([f{kwargs["on_change"].cast<py::function>()}](int index)
            {
                py::gil_scoped_acquire acquire;
                f(index);
            });
        }
    }

    void Definition<ComboBox>::apply(py::module& module)
    {
        py::class_<ComboBox, Node, std::shared_ptr<ComboBox>> combo(module, "ComboBox");

        combo.def(py::init<>([](py::kwargs kwargs) {
            auto combo = std::make_shared<ComboBox>();
            parse(kwargs, *combo);
            return combo;
        }));

        combo.def_property("on_change", &ComboBox::on_change, [](ComboBox& check_box, ComboBox::OnChange f) {
            check_box.set_on_change([f{ std::move(f) }](bool value) {
                py::gil_scoped_acquire acquire;
                f(value);
            });
        });
        combo.def_property("options", &ComboBox::options, &ComboBox::set_options);
        combo.def_property("selected_index", &ComboBox::selected_index, &ComboBox::set_selected_index);
        combo.def_property("hint", &ComboBox::hint, &ComboBox::set_hint);
    }

}