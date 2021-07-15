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
#include <p3/ScrollArea.h>


namespace p3::python
{

    void Definition<ScrollArea>::parse(py::kwargs const& kwargs, ScrollArea& scroll_area)
    {
        Definition<Node>::parse(kwargs, scroll_area);
    }

    void Definition<ScrollArea>::apply(py::module& module)
    {
        py::class_<ScrollArea, Node, std::shared_ptr<ScrollArea>> scroll_area(module, "ScrollArea");

        scroll_area.def(py::init<>([](py::kwargs kwargs) {
            auto scroll_area = std::make_shared<ScrollArea>();
            parse(kwargs, *scroll_area);
            if (kwargs.contains("horizontal_scroll_enabled"))
                scroll_area->set_horizontal_scroll_enabled(kwargs["horizontal_scroll_enabled"].cast<bool>());
            if (kwargs.contains("content"))
                scroll_area->set_content(kwargs["content"].cast<std::shared_ptr<Node>>());
            return scroll_area;
        }));

        scroll_area.def_property("content", &ScrollArea::content, &ScrollArea::set_content);
    }

}
