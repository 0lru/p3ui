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
    ArgumentParser<Layout>()(kwargs, layout);
    assign(kwargs, "spacing", layout, &Layout::set_spacing);
    assign(kwargs, "padding", layout, &Layout::set_padding);
    assign(kwargs, "direction", layout, &Layout::set_direction);
    assign(kwargs, "align_items", layout, &Layout::set_align_items);
    assign(kwargs, "justify_content", layout, &Layout::set_justify_content);
}

void Definition<Layout>::apply(py::module& m)
{
    py::class_<Layout, Node, std::shared_ptr<Layout>> layout(m, "Layout", R"doc(
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
    def_property(layout, "justify_content", &Layout::justify_content, &Layout::set_justify_content);

    py::class_<Row, Layout, std::shared_ptr<Row>>(m, "Row").def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Row>();
        ArgumentParser<Layout>()(kwargs, *layout);
        return layout;
    }));
    py::class_<Column, Layout, std::shared_ptr<Column>>(m, "Column").def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Column>();
        ArgumentParser<Layout>()(kwargs, *layout);
        return layout;
    }));
}

}
