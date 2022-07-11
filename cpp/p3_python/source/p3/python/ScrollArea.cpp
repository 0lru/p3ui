#include "p3ui.h"
#include <p3/widgets/ScrollArea.h>

namespace p3::python {

void Definition<ScrollArea>::apply(py::module& module)
{
    py::class_<ScrollArea, Node, std::shared_ptr<ScrollArea>> scroll_area(module, "ScrollArea");

    scroll_area.def(py::init<>([](
                                   std::optional<std::shared_ptr<Node>> content,
                                   std::optional<bool> horizontal_scroll_enabled,
                                   std::optional<bool> vertical_scroll_autohide,
                                   std::optional<bool> horizontal_scroll_autohide,
                                   py::kwargs kwargs) {
        auto scroll_area = std::make_shared<ScrollArea>();
        ArgumentParser<Node>()(kwargs, *scroll_area);

        auto dict = (*std::static_pointer_cast<py::dict>(scroll_area->user_data()));
        (*dict)["content"] = py::cast(content);
        assign(content, *scroll_area, &ScrollArea::set_content);

        assign(horizontal_scroll_enabled, *scroll_area, &ScrollArea::set_horizontal_scroll_enabled);
        assign(horizontal_scroll_autohide, *scroll_area, &ScrollArea::set_horizontal_scroll_autohide);
        assign(vertical_scroll_autohide, *scroll_area, &ScrollArea::set_vertical_scroll_autohide);
        assign(kwargs, "on_content_region_changed", *scroll_area, &p3::ScrollArea::set_on_content_region_changed);
        assign(kwargs, "content_size", *scroll_area, &p3::ScrollArea::set_content_size);
        return scroll_area;
    }),
        py::kw_only(),
        py::arg("content") = py::none(),
        py::arg("horizontal_scroll_enabled") = py::none(),
        py::arg("vertical_scroll_autohide") = py::none(),
        py::arg("horizontal_scroll_autohide") = py::none());

    def_signal_property(scroll_area, "on_content_region_changed", &ScrollArea::on_content_region_changed, &ScrollArea::set_on_content_region_changed);
    def_content_property(scroll_area, "content", &ScrollArea::content, &ScrollArea::set_content);
    scroll_area.def_property("vertical_scroll_autohide", &ScrollArea::vertical_scroll_autohide, &ScrollArea::set_vertical_scroll_autohide);
    scroll_area.def_property("horizontal_scroll_autohide", &ScrollArea::horizontal_scroll_autohide, &ScrollArea::set_horizontal_scroll_autohide);
    def_property(scroll_area, "scroll_x", &ScrollArea::scroll_x, &ScrollArea::set_scroll_x);
    def_property(scroll_area, "scroll_y", &ScrollArea::scroll_y, &ScrollArea::set_scroll_y);
    def_property(scroll_area, "content_size", &ScrollArea::content_size, &ScrollArea::set_content_size);
    def_property_readonly(scroll_area, "content_region", &ScrollArea::content_region);
}

}
