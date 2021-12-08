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