
#include "p3ui.h"
#include <p3/Text.h>


namespace p3::python
{

    void Definition<Text>::apply(py::module& module)
    {
        py::class_<Text, Node, std::shared_ptr<Text>> text(module, "Text");

        text.def(py::init<>([](std::string value, py::kwargs kwargs) {
            auto text = std::make_shared<Text>(std::move(value));
            ArgumentParser<Node>()(kwargs, *text);
            return text;
        }), py::arg("value")=std::string());
        def_property(text, "value", &Text::value, &Text::set_value);
    }

}