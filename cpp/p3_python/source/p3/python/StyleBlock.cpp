#include "p3ui.h"

namespace p3::python {
/*
void ArgumentParser<StyleBlock>::operator()(py::kwargs const& kwargs, StyleBlock& style_block)
{
    assign(kwargs, "position", style_block, &StyleBlock::set_position);
    assign(kwargs, "color", style_block, &StyleBlock::set_color);
    assign(kwargs, "background_color", style_block, &StyleBlock::set_background_color);
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
    assign(kwargs, "border_width", style_block, &StyleBlock::set_border_width);
    assign(kwargs, "border_color", style_block, &StyleBlock::set_border_color);
}

void Definition<StyleBlock>::apply(py::module& module)
{
     py::class_<StyleBlock, std::shared_ptr<StyleBlock>> style(module, "Style");

    style.def(py::init<>([](py::kwargs& kwargs) {
        auto style = std::make_shared<StyleBlock>();
        ArgumentParser<StyleBlock>()(kwargs, *style);
        return style;
    }));

 def_property(style, "position", &StyleBlock::position, &StyleBlock::set_position);
    def_property(style, "background_color", &StyleBlock::color, &StyleBlock::set_background_color);
    def_property(style, "x", &StyleBlock::x, &StyleBlock::set_x);
    def_property(style, "y", &StyleBlock::y, &StyleBlock::set_y);
    def_property(style, "width", &StyleBlock::width, &StyleBlock::set_width);
    def_property(style, "height", &StyleBlock::height, &StyleBlock::set_height);
    def_property(style, "visible", &StyleBlock::visible, &StyleBlock::set_visible);
    def_property(style, "border_width", &StyleBlock::border_width, &StyleBlock::set_border_width);
    def_property(style, "border_color", &StyleBlock::border_color, &StyleBlock::set_border_color);
}
    */

}