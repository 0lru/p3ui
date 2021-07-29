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
#include <p3/InputText.h>


namespace p3::python
{

    void Definition<InputText>::parse(py::kwargs const& kwargs, InputText& input)
    {
        Definition<Node>::parse(kwargs, input);
        if (kwargs.contains("hint"))
            input.set_hint(kwargs["hint"].cast<std::optional<std::string>>());
        if (kwargs.contains("on_change"))
        {
            input.set_on_change([f{kwargs["on_change"].cast<py::function>()}]()
            {
                py::gil_scoped_acquire acquire;
                f();
            });
        }
    }

    void Definition<InputText>::apply(py::module& module)
    {
        py::class_<InputText, Node, std::shared_ptr<InputText>> input(module, "InputText");

        input.def(py::init<>([](std::size_t size, py::kwargs kwargs) {
            auto input = std::make_shared<InputText>(size);
            parse(kwargs, *input);
            return input;
        }), py::arg("size")=16);

        input.def(py::init<>([](py::kwargs kwargs) {
            auto input = std::make_shared<InputText>();
            parse(kwargs, *input);
            return input;
        }));

        input.def_property("on_change", &InputText::on_change, &InputText::set_on_change);
        input.def_property("value", &InputText::value, &InputText::set_value);
    }

}
