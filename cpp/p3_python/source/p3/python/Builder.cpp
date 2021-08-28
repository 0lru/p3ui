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

#include <p3/log.h>
#include <p3/Node.h>
#include <p3/Builder.h>

#include <regex>

#include <iostream>

namespace p3::python
{

    class Builder : public p3::Builder
    {
    public:
        std::string bind(py::function f)
        {
            log_info("about to bind function (id={})", _callbacks.size());
            _callbacks.push_back(f);
            return "{" + fmt::format("cb{}", _callbacks.size() - 1) + "}";
        }

    protected:
        void assign_attribute(Node& node, std::string const& name, std::string const& value) override
        {
            log_info("assign {} to {}", name, value);
            static std::regex e(R"(^\{cb([\d]+)\})");
            std::cmatch match;
            if (std::regex_match(value.c_str(), match, e))
            {
                log_info(R"(bounds "{}" with (id={}))", name, value);
                py::object object = py::cast(std::static_pointer_cast<Node>(node.shared_from_this()));
                py::setattr(object, name.c_str(), _callbacks[std::stoi(match[1].str())]);
            }
            else
            {
                p3::Builder::assign_attribute(node, name, value);
            }
        }

    private:
        std::vector<py::function> _callbacks;
    };

    void Definition<Builder>::apply(py::module& module)
    {
        py::class_<Builder, std::shared_ptr<Builder>> builder(module, "Builder");

        builder.def(py::init<>([]() {
            return std::make_shared<Builder>();
        }));

        builder.def("build", [](Builder& builder, std::string const& xml_text) {
            return builder.build(xml_text);
        });

        builder.def("bind", &Builder::bind);
    }

}