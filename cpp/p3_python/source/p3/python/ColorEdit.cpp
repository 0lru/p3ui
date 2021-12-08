
#include "p3ui.h"
#include <p3/ColorEdit.h>

namespace p3::python
{

    void Definition<ColorEdit>::apply(py::module& module)
    {
        py::class_<ColorEdit, Node, std::shared_ptr<ColorEdit>> color_edit(module, "ColorEdit");
        color_edit.def(py::init<>([](std::optional<py::function> on_change, std::optional<Color> value, py::kwargs kwargs) {
            auto color_edit = std::make_shared<ColorEdit>();
            ArgumentParser<Node>()(kwargs, *color_edit);
            assign(on_change, *color_edit, &ColorEdit::set_on_change);
            assign(value, *color_edit, &ColorEdit::set_value);
            return color_edit;
        }), py::kw_only(), py::arg("on_change")=py::none(), py::arg("value")=py::none());
        def_property(color_edit, "on_change", &ColorEdit::on_change, &ColorEdit::set_on_change);
        def_property(color_edit, "value", &ColorEdit::value, &ColorEdit::set_value);
    }

}