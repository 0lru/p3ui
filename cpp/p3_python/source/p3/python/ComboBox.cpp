#include "p3ui.h"

#include <p3/widgets/ComboBox.h>


namespace p3::python
{

    void Definition<ComboBox>::apply(py::module& module)
    {
        py::class_<ComboBox, Node, std::shared_ptr<ComboBox>> combo(module, "ComboBox");

        combo.def(py::init<>([](py::kwargs kwargs) {
            auto combo = std::make_shared<ComboBox>();
            ArgumentParser<Node>()(kwargs, *combo);
            assign(kwargs, "options", *combo, &ComboBox::set_options);
            assign(kwargs, "selected_index", *combo, &ComboBox::set_selected_index);
            assign(kwargs, "on_change", *combo, &ComboBox::set_on_change);
            return combo;
        }));

        def_property(combo, "on_change", &ComboBox::on_change, &ComboBox::set_on_change);
        def_property(combo, "options", &ComboBox::options, &ComboBox::set_options);
        def_property(combo, "selected_index", &ComboBox::selected_index, &ComboBox::set_selected_index);
        def_property(combo, "hint", &ComboBox::hint, &ComboBox::set_hint);
    }

}