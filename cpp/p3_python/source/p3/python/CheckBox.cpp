#include "p3ui.h"

#include <p3/widgets/CheckBox.h>


namespace p3::python
{

    void Definition<CheckBox>::apply(py::module& module)
    {
        py::class_<CheckBox, Node, std::shared_ptr<CheckBox>> check_box(module, "CheckBox");

        check_box.def(py::init<>([](std::optional<py::function> on_change, std::optional<bool> value, std::optional<bool> radio, py::kwargs kwargs) {
            auto check_box = std::make_shared<CheckBox>();
            ArgumentParser<Node>()(kwargs, *check_box);
            assign(on_change, *check_box, &CheckBox::set_on_change);
            assign(value, *check_box, &CheckBox::set_value);
            assign(radio, *check_box, &CheckBox::set_radio);
            return check_box;
        }), py::kw_only(), py::arg("on_change")=py::none(), py::arg("value")=py::none(), py::arg("radio")=py::none());

        def_property(check_box, "on_change", &CheckBox::on_change, &CheckBox::set_on_change);
        def_property(check_box, "value", &CheckBox::value, &CheckBox::set_value);
        def_property(check_box, "radio", &CheckBox::radio, &CheckBox::set_radio);
    }

}