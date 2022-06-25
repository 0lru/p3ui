#include "p3ui.h"

#include <p3/Layout.h>

namespace p3::python {

class Row : public Layout {
public:
    Row()
        : Layout()
    {
        style()->set_direction(Direction::Horizontal);
    }
};

class Column : public Layout {
public:
    Column()
        : Layout()
    {
        style()->set_direction(Direction::Vertical);
    }
};

void Definition<Layout>::apply(py::module& m)
{
    py::class_<Layout, Node, std::shared_ptr<Layout>> layout(m, "Layout", R"doc(
            :py:class:`Layout` Partial Adaption of the CSS flexbox.
        )doc");

    layout.def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Layout>();
        ArgumentParser<Node>()(kwargs, *layout);
        return layout;
    }));

    def_method(layout, "add", &Layout::add);
    def_method(layout, "insert", &Layout::insert);
    def_method(layout, "remove", &Layout::remove);

    py::class_<Row, Layout, std::shared_ptr<Row>>(m, "Row").def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Row>();
        ArgumentParser<Node>()(kwargs, *layout);
        return layout;
    }));
    py::class_<Column, Layout, std::shared_ptr<Column>>(m, "Column").def(py::init<>([](py::kwargs kwargs) {
        auto layout = std::make_shared<Column>();
        ArgumentParser<Node>()(kwargs, *layout);
        return layout;
    }));
}

}
