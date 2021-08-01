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
#include <p3/Collapsible.h>

namespace p3::python
{

    void Definition<Collapsible>::apply(py::module& module)
    {
        py::class_<Collapsible, Node, std::shared_ptr<Collapsible>> collapsible(module, "Collapsible");
        
        collapsible.def(py::init<>([](std::optional<std::shared_ptr<Node>> content, std::optional<bool> collapsed, py::kwargs kwargs) {
            auto collapsible = std::make_shared<Collapsible>("");
            ArgumentParser<Node>()(kwargs, *collapsible);
            assign(content, *collapsible, &Collapsible::set_content);
            assign(collapsed, *collapsible, &Collapsible::set_collapsed);
            return collapsible;
        }), py::kw_only(), py::arg("content")=py::none(), py::arg("collapsed")=true);
        def_property(collapsible, "content", &Collapsible::content, &Collapsible::set_content);
        def_property(collapsible, "collapsed", &Collapsible::collapsed, &Collapsible::set_collapsed);
    }

}