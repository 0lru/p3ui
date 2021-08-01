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

#include <p3/Layout.h>


namespace p3::python
{

    void Definition<Layout>::apply(py::module& module)
    {
        py::class_<Layout, Node, std::shared_ptr<Layout>> flexible(module, "Layout", R"doc(
            :py:class:`Layout` Partial Adaption of the CSS flexbox.
        )doc");

        flexible.def(py::init<>([](py::kwargs kwargs) {
            auto flexible = std::make_shared<Layout>();
            ArgumentParser<Node>()(kwargs, *flexible);
            return flexible;
        }));

        def_method(flexible, "add", &Layout::add);
        def_method(flexible, "insert", &Layout::insert);
        def_method(flexible, "remove", &Layout::remove);
    }

}
