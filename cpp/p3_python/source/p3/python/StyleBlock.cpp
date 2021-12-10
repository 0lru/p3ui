#include "p3ui.h"

#include <p3/StyleBlock.h>


namespace p3::python
{

    void ArgumentParser<StyleBlock>::operator()(py::kwargs const& kwargs, StyleBlock& style_block)
    {
        assign(kwargs, "position", style_block, &StyleBlock::set_position);
        assign(kwargs, "color", style_block, &StyleBlock::set_color);
        assign(kwargs, "spacing", style_block, &StyleBlock::set_spacing);
        assign(kwargs, "padding", style_block, &StyleBlock::set_padding);
        assign(kwargs, "x", style_block, &StyleBlock::set_x);
        assign(kwargs, "y", style_block, &StyleBlock::set_y);
        assign(kwargs, "width", style_block, &StyleBlock::set_width);
        assign(kwargs, "height", style_block, &StyleBlock::set_height);
        assign(kwargs, "visible", style_block, &StyleBlock::set_visible);
        assign(kwargs, "direction", style_block, &StyleBlock::set_direction);
        assign(kwargs, "align_items", style_block, &StyleBlock::set_align_items);
        assign(kwargs, "justify_content", style_block, &StyleBlock::set_justify_content);
    }

    void Definition<StyleBlock>::apply(py::module& module)
    {
        auto asyncio = py::module::import("asyncio");

        py::class_<StyleBlock, std::shared_ptr<StyleBlock>> style(module, "Style");

        style.def(py::init<>([](py::kwargs& kwargs) {
            auto style = std::make_shared<StyleBlock>();
            ArgumentParser<StyleBlock>()(kwargs, *style);
            return style;
        }));

        def_property(style, "position", &StyleBlock::position, &StyleBlock::set_position);
        def_property(style, "color", &StyleBlock::color, &StyleBlock::set_color);
        def_property(style, "spacing", &StyleBlock::spacing, &StyleBlock::set_spacing);
        def_property(style, "padding", &StyleBlock::padding, &StyleBlock::set_padding);
        def_property(style, "x", &StyleBlock::x, &StyleBlock::set_x);
        def_property(style, "y", &StyleBlock::y, &StyleBlock::set_y);
        def_property(style, "width", &StyleBlock::width, &StyleBlock::set_width);
        def_property(style, "height", &StyleBlock::height, &StyleBlock::set_height);
        def_property(style, "visible", &StyleBlock::visible, &StyleBlock::set_visible);
        def_property(style, "direction", &StyleBlock::direction, &StyleBlock::set_direction);
        def_property(style, "align_items", &StyleBlock::align_items, &StyleBlock::set_align_items);
        def_property(style, "justify_content", &StyleBlock::justify_content, &StyleBlock::set_justify_content);

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

        //
        // em
        py::class_<Em>(module, "Em")
            .def(py::init<>([](double value) { return Em{ static_cast<float>(value) }; }))
            .def("__float__", [](Em const& self) { return self.value; })
            .def_readwrite("value", &Em::value);

        py::class_<UnitType<Em>>(module, "EmUnit")
            .def("__ror__", [](UnitType<Em> const&, double value) {return Em{ static_cast<float>(value) }; });

        module.attr("em") = UnitType<Em>();

        //
        // em
        py::class_<Rem>(module, "Rem")
            .def(py::init<>([](double value) { return Rem{ static_cast<float>(value) }; }))
            .def("__float__", [](Rem const& self) { return self.value; })
            .def_readwrite("value", &Rem::value);

        py::class_<UnitType<Rem>>(module, "RemUnit")
            .def("__ror__", [](UnitType<Rem> const&, double value) {return Rem{ static_cast<float>(value) }; });

        module.attr("rem") = UnitType<Rem>();

        //
        // px
        py::class_<Px>(module, "Px")
            .def(py::init<>([](double value) { return Px{ static_cast<float>(value) }; }))
            .def("__float__", [](Px const& self) { return self.value; })
            .def_readwrite("value", &Px::value);

        py::class_<UnitType<Px>>(module, "PxUnit")
            .def("__ror__", [](UnitType<Px> const&, double value) {return Px{ static_cast<float>(value) }; });

        module.attr("px") = UnitType<Px>();

        //
        // percent
        py::class_<Percentage>(module, "Percentage")
            .def(py::init<>([](double value) { return Percentage{ static_cast<float>(value) }; }))
            .def("__float__", [](Percentage const& self) { return self.value; })
            .def_readwrite("value", &Percentage::value);

        py::class_<UnitType<Percentage>>(module, "PercentageUnit")
            .def("__ror__", [](UnitType<Percentage> const&, double value) {return Percentage{ static_cast<float>(value) }; });

        module.attr("percent") = UnitType<Percentage>();

        //
        // auto
        py::class_<std::nullptr_t> automatic(module, "Automatic");
        module.attr("auto") = std::nullopt;
    }

}