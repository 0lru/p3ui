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

    class Row : public Layout
    {
    public:
        Row() : Layout()
        {
            style()->set_direction(Direction::Horizontal);
        }
    };

    class Column : public Layout
    {
    public:
        Column() : Layout()
        {
            style()->set_direction(Direction::Vertical);
        }
    };

    void Definition<Layout>::apply(py::module& m)
    {
        py::class_<Layout, Node, std::shared_ptr<Layout>> layout(m, "Layout", R"doc(
            :py:class:`Layout` Partial Adaption of the CSS flexbox.
        )doc");

        layout.def(py::init<>([](py::kwargs kwargs) {
            auto layout = std::make_shared<Layout>();
            ArgumentParser<Node>()(kwargs, *layout);
            return layout;
        }));

        def_method(layout, "add", &Layout::add);
        def_method(layout, "insert", &Layout::insert);
        def_method(layout, "remove", &Layout::remove);

        py::class_<Row, Layout, std::shared_ptr<Row>>(m, "Row").def(py::init<>([](py::kwargs kwargs) {
            auto layout = std::make_shared<Row>();
            ArgumentParser<Node>()(kwargs, *layout);
            return layout;
        }));
        py::class_<Column, Layout, std::shared_ptr<Column>>(m, "Column").def(py::init<>([](py::kwargs kwargs) {
            auto layout = std::make_shared<Column>();
            ArgumentParser<Node>()(kwargs, *layout);
            return layout;
        }));
    }

}