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
#include <p3/Node.h>


namespace p3::python
{

    void Definition<Node>::parse(py::kwargs const& kwargs, Node& node)
    {
        if (kwargs.contains("label"))
            node.set_label(kwargs["label"].cast<std::optional<std::string>>());
        if (kwargs.contains("style"))
            node.set_style(kwargs["style"].cast<std::shared_ptr<StyleBlock>>());
        if (kwargs.contains("color"))
            node.style()->set_color(kwargs["color"].cast<Color>());
        if (kwargs.contains("spacing"))
            node.style()->set_spacing(kwargs["spacing"].cast<Length2>());
        if (kwargs.contains("padding"))
            node.style()->set_padding(kwargs["padding"].cast<Length2>());
        if (kwargs.contains("x"))
            node.style()->set_x(kwargs["x"].cast<Length>());
        if (kwargs.contains("y"))
            node.style()->set_y(kwargs["y"].cast<Length>());
        if (kwargs.contains("width"))
            node.style()->set_width(kwargs["width"].cast<FlexibleLength>());
        if (kwargs.contains("height"))
            node.style()->set_height(kwargs["height"].cast<FlexibleLength>());
        if (kwargs.contains("visible"))
            node.style()->set_visible(kwargs["visible"].cast<bool>());
        if (kwargs.contains("direction"))
            node.style()->set_direction(kwargs["direction"].cast<Direction>());
        if (kwargs.contains("align_items"))
            node.style()->set_align_items(kwargs["align_items"].cast<Alignment>());
        if (kwargs.contains("justify_content"))
            node.style()->set_justify_content(kwargs["justify_content"].cast<Justification>());
        if (kwargs.contains("visible"))
            node.set_visible(kwargs["visible"].cast<bool>());
        if (kwargs.contains("on_mouse_enter"))
            node.set_on_mouse_enter([f{ kwargs["on_mouse_enter"].cast<py::function>() }](Node::MouseEvent e) {
            py::gil_scoped_acquire acquire;
            f(std::move(e));
        });
        if (kwargs.contains("on_mouse_move"))
            node.set_on_mouse_move([f{ kwargs["on_mouse_move"].cast<py::function>() }](Node::MouseEvent e) {
            py::gil_scoped_acquire acquire;
            f(std::move(e));
        });
        if (kwargs.contains("on_mouse_leave"))
            node.set_on_mouse_leave([f{ kwargs["on_mouse_leave"].cast<py::function>() }](Node::MouseEvent e) {
            py::gil_scoped_acquire acquire;
            f(std::move(e));
        });
        if (kwargs.contains("children"))
        {
            auto children = kwargs["children"].cast<std::vector<std::shared_ptr<Node>>>();
            for (auto& child : children)
                node.add(child);
        }
        if (kwargs.contains("disabled"))
            node.set_disabled(kwargs["disabled"].cast<bool>());
    }

    void Definition<Node>::apply(py::module& module)
    {
        py::enum_<Direction>(module, "Direction")
            .value("Horizontal", Direction::Horizontal)
            .value("Vertical", Direction::Vertical)
            .export_values();

        py::enum_<Alignment>(module, "Alignment")
            .value("Start", Alignment::Start)
            .value("Center", Alignment::Center)
            .value("End", Alignment::End)
            .value("Stretch", Alignment::Stretch)
            .value("Baseline", Alignment::Center) // for now ..
            .export_values();

        py::enum_<Justification>(module, "Justification")
            .value("Start", Justification::Start)
            .value("Center", Justification::Center)
            .value("End", Justification::End)
            .value("SpaceAround", Justification::SpaceAround)
            .value("SpaceBetween", Justification::SpaceBetween)
            .export_values();

        py::class_<Node::MouseEvent>(module, "MouseEvent")
            .def_property_readonly("source", [](Node::MouseEvent& e) {
            return e.source()->shared_from_this();
        })
            .def_property_readonly("x", &Node::MouseEvent::x)
            .def_property_readonly("y", &Node::MouseEvent::y)
            .def_property_readonly("global_x", &Node::MouseEvent::global_x)
            .def_property_readonly("global_y", &Node::MouseEvent::global_y);

        py::class_<Node, std::shared_ptr<Node>> node(module, "Node");
        node.def_property("visible", &Node::visible, &Node::set_visible);
        node.def_property_readonly("children", &Node::children);
        node.def_property("disabled", &Node::disabled, &Node::set_disabled);
        node.def_property("label", &Node::label, &Node::set_label);
        node.def_property("style", &Node::style, &Node::set_style);
        node.def_property("on_mouse_enter", &Node::on_mouse_enter, [](Node& node, py::function f) {
            node.set_on_mouse_enter([f{ std::move(f) }](Node::MouseEvent e) {
                py::gil_scoped_acquire acquire;
                f(std::move(e));
            });
        });
        node.def_property("on_mouse_move", &Node::on_mouse_move, [](Node& node, py::function f) {
            node.set_on_mouse_move([f{ std::move(f) }](Node::MouseEvent e) {
                py::gil_scoped_acquire acquire;
                f(std::move(e));
            });
        });
        node.def_property("on_mouse_leave", &Node::on_mouse_leave, [](Node& node, py::function f) {
            node.set_on_mouse_leave([f{ std::move(f) }](Node::MouseEvent e) {
                py::gil_scoped_acquire acquire;
                f(std::move(e));
            });
        });


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

        py::class_<UnitType<Rem>>(module, "REmUnit")
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
        py::class_<std::nullptr_t>(module, "Automatic");
        module.attr("auto") = std::nullopt;
    }

}
