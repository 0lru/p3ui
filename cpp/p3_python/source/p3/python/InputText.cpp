#include "p3ui.h"

#include <p3/widgets/InputText.h>

namespace p3::python {

void ArgumentParser<InputText>::operator()(py::kwargs const& kwargs, InputText& input_text)
{
    ArgumentParser<Node>()(kwargs, input_text);
    assign(kwargs, "hint", input_text, &InputText::set_hint);
    assign(kwargs, "multi_line", input_text, &InputText::set_multi_line);
    assign(kwargs, "on_change", input_text, &InputText::set_on_change);
}

void Definition<InputText>::apply(py::module& module)
{
    py::class_<InputText, Node, std::shared_ptr<InputText>> input(module, "InputText");

    input.def(py::init<>([](std::size_t size, py::kwargs kwargs) {
        auto input = std::make_shared<InputText>(size);
        ArgumentParser<InputText>()(kwargs, *input);
        return input;
    }),
        py::kw_only(), py::arg("size") = 16);

    def_signal_property(input, "on_change", &InputText::on_change, &InputText::set_on_change);
    def_property(input, "value", &InputText::value, &InputText::set_value);
    def_property(input, "multi_line", &InputText::multi_line, &InputText::set_multi_line);
}

}
