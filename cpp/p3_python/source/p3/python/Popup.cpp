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
#include <p3/Popup.h>


namespace p3::python
{

    void Definition<Popup>::parse(py::kwargs const& kwargs, Popup& popup)
    {
        Definition<Node>::parse(kwargs, popup);
        if (kwargs.contains("content"))
            popup.set_content(kwargs["content"].cast<std::shared_ptr<Node>>());
    }

    void Definition<Popup>::apply(py::module& module)
    {
        py::class_<Popup, Node, std::shared_ptr<Popup>> popup(module, "Popup");
        popup.def(py::init<>([](py::kwargs kwargs) {
            auto popup = std::make_shared<Popup>();
            parse(kwargs, *popup);
            return popup;
        }));
        popup.def_property("content", &Popup::content, &Popup::set_content);
    }

}