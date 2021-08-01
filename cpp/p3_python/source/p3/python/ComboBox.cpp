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

    void Definition<ComboBox>::apply(py::module& module)
    {
        py::class_<ComboBox, Node, std::shared_ptr<ComboBox>> combo(module, "ComboBox");

        combo.def(py::init<>([](py::kwargs kwargs) {
            auto combo = std::make_shared<ComboBox>();
            ArgumentParser<Node>()(kwargs, *combo);
            assign(kwargs, "options", *combo, &ComboBox::set_options);
            assign(kwargs, "selected_index", *combo, &ComboBox::set_selected_index);
            assign(kwargs, "on_change", *combo, &ComboBox::set_on_change);
            return combo;
        }));

        def_property(combo, "on_change", &ComboBox::on_change, &ComboBox::set_on_change);
        def_property(combo, "options", &ComboBox::options, &ComboBox::set_options);
        def_property(combo, "selected_index", &ComboBox::selected_index, &ComboBox::set_selected_index);
        def_property(combo, "hint", &ComboBox::hint, &ComboBox::set_hint);
    }

}