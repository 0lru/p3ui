#include "p3ui.h"

#include <p3/layout.h>

namespace p3::python {

class Row : public Layout {
public:
    Row()
        : Layout()
    {
        set_direction(Direction::Horizontal);
    }
};

class Column : public Layout {
public:
    Column()
        : Layout()
    {
        set_direction(Direction::Vertical);
    }
};

void ArgumentParser<Layout>::operator()(py::kwargs const& kwargs, Layout& layout)
{
    ArgumentParser<Node>()(kwargs, layout);
    assign(kwargs, "spacing", layout, &Layout::set_spacing);
    assign(kwargs, "padding", layout, &Layout::set_padding);
    assign(kwargs, "direction", layout, &Layout::set_direction);
    assign(kwargs, "align_items", layout, &Layout::set_align_items);
    assign(kwargs, "justify_content", layout, &Layout::set_justify_content);
    assign(kwargs, "background_color", layout, &Layout::set_background_color);
}

void Definition<Layout>::apply(py::module& module)
{
    py::class_<Em>(module, "Em")
        .def(py::init<>([](double value) { return Em { static_cast<float>(value) }; }))
        .def("__float__", [](Em const& self) { return self.value; })
        .def_readwrite("value", &Em::value);
    py::class_<UnitType<Em>>(module, "EmUnit")
        .def("__ror__", [](UnitType<Em> const&, double value) { return Em { static_cast<float>(value) }; });
    module.attr("em") = UnitType<Em>();

    py::class_<Rem>(module, "Rem")
        .def(py::init<>([](double value) { return Rem { static_cast<float>(value) }; }))
        .def("__float__", [](Rem const& self) { return self.value; })
        .def_readwrite("value", &Rem::value);
    py::class_<UnitType<Rem>>(module, "RemUnit")
        .def("__ror__", [](UnitType<Rem> const&, double value) { return Rem { static_cast<float>(value) }; });
    module.attr("rem") = UnitType<Rem>();

    py::class_<Px>(module, "Px")
        .def(py::init<>([](double value) { return Px { static_cast<float>(value) }; }))
        .def("__float__", [](Px const& self) { return self.value; })
        .def_readwrite("value", &Px::value);
    py::class_<UnitType<Px>>(module, "PxUnit")
        .def("__ror__", [](UnitType<Px> const&, double value) { return Px { static_cast<float>(value) }; });
    module.attr("px") = UnitType<Px>();

    py::class_<Percentage>(module, "Percentage")
        .def(py::init<>([](double value) { return Percentage { static_cast<float>(value) }; }))
        .def("__float__", [](Percentage const& self) { return self.value; })
        .def_readwrite("value", &Percentage::value);
    py::class_<UnitType<Percentage>>(module, "PercentageUnit")
        .def("__ror__", [](UnitType<Percentage> const&, double value) { return Percentage { static_cast<float>(value) }; });
    module.attr("percent") = UnitType<Percentage>();

    py::class_<std::nullptr_t> automatic(module, "Automatic");
    module.attr("auto") = std::nullopt;

    py::enum_<Direction>(module, "Direction")
        .value("Horizontal", Direction::Horizontal)
        .value("Vertical", Direction::Vertical)
        .export_values();

    py::enum_<Position>(module, "Position")
        .value("Static", Position::Static)
        .value("Absolute", Position::Absolute)
        .export_values();

    py::enum_<Alignment>(module, "Alignment")
        .value("Start", Alignment::Start)
        .value("Center", Alignment::Center)
        .value("End", Alignment::End)
        .value("Stretch", Alignment::Stretch)
        .value("Baseline", Alignment::Center) // "Center" for now ..
        .export_values();

    py::enum_<Justification>(module, "Justification")
        .value("Start", Justification::Start)
        .value("Center", Justification::Center)
        .value("End", Justification::End)
        .value("SpaceAround", Justification::SpaceAround)
        .value("SpaceBetween", Justification::SpaceBetween)
        .export_values();

    py::class_<Layout, Node, std::shared_ptr<Layout>> layout(module, "Layout", R"doc(
            :py:class:` CSS-flexbox-like container.
        )doc");

    layout.def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Layout>();
        ArgumentParser<Layout>()(kwargs, *layout);
        return layout;
    }));
    def_property(layout, "spacing", &Layout::spacing, &Layout::set_spacing);
    def_property(layout, "padding", &Layout::padding, &Layout::set_padding);
    def_property(layout, "direction", &Layout::direction, &Layout::set_direction);
    def_property(layout, "align_items", &Layout::align_items, &Layout::set_align_items);
    def_property(layout, "background_color", &Layout::background_color, &Layout::set_background_color);
    def_property(layout, "justify_content", &Layout::justify_content, &Layout::set_justify_content);

    py::class_<Row, Layout, std::shared_ptr<Row>>(module, "Row").def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Row>();
        ArgumentParser<Layout>()(kwargs, *layout);
        return layout;
    }));
    py::class_<Column, Layout, std::shared_ptr<Column>>(module, "Column").def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Column>();
        ArgumentParser<Layout>()(kwargs, *layout);
        return layout;
    }));
}

}
