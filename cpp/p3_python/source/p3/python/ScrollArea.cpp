
#include "p3ui.h"
#include <p3/ScrollArea.h>


namespace p3::python
{

    void Definition<ScrollArea>::apply(py::module& module)
    {
        py::class_<ScrollArea, Node, std::shared_ptr<ScrollArea>> scroll_area(module, "ScrollArea");

        scroll_area.def(py::init<>([](std::optional<std::shared_ptr<Node>> content, std::optional<bool> horizontal_scroll_enabled, py::kwargs kwargs) {
            auto scroll_area = std::make_shared<ScrollArea>();
            ArgumentParser<Node>()(kwargs, *scroll_area);
            assign(content, *scroll_area, &ScrollArea::set_content);
            assign(horizontal_scroll_enabled, *scroll_area, &ScrollArea::set_horizontal_scroll_enabled);
            return scroll_area;
        }), py::kw_only(), py::arg("content") = py::none(), py::arg("horizontal_scroll_enabled") = py::none());

        scroll_area.def_property("content", &ScrollArea::content, &ScrollArea::set_content);
    }

}
