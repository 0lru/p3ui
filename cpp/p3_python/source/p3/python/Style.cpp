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
#include <p3/StyleBlock.h>

namespace p3::python
{

    void Definition<StyleBlock>::parse(py::kwargs const& kwargs, StyleBlock& button)
    {
    }

    void Definition<StyleBlock>::apply(py::module& module)
    {
        py::class_<StyleBlock, std::shared_ptr<StyleBlock>> style(module, "Style");
        style.def(py::init<>([](
            std::optional<Color> color,
            std::optional<Length> border_width,
            std::optional<Length> border_radius,
            std::optional<Length2> spacing,
            std::optional<Length2> padding,
            std::optional<LengthPercentage> x,
            std::optional<LengthPercentage> y,
            std::optional<LayoutLength> width,
            std::optional<LayoutLength> height,
            std::optional<bool> visible,
            std::optional<Direction> direction,
            std::optional<Alignment> align_items,
            std::optional<Justification> justify_content
            ) {
            auto style = std::make_shared<StyleBlock>();
            style->set_color(std::move(color));
            style->set_spacing(std::move(spacing));
            style->set_padding(std::move(padding));
            style->set_border_width(std::move(border_width));
            style->set_border_radius(std::move(border_radius));
            style->set_spacing(std::move(spacing));
            style->set_x(std::move(x));
            style->set_y(std::move(y));
            style->set_width(std::move(width));
            style->set_height(std::move(height));
            style->set_visible(std::move(visible));
            style->set_direction(std::move(direction));
            style->set_align_items(std::move(align_items));
            style->set_justify_content(std::move(justify_content));
            return style;
        }), py::arg("color") = std::nullopt,
            py::arg("border_width") = std::nullopt,
            py::arg("border_radius") = std::nullopt,
            py::arg("spacing") = std::nullopt,
            py::arg("padding") = std::nullopt,
            py::arg("x") = std::nullopt,
            py::arg("y") = std::nullopt,
            py::arg("width") = std::nullopt,
            py::arg("height") = std::nullopt,
            py::arg("visible") = std::nullopt,
            py::arg("direction") = std::nullopt,
            py::arg("align_items") = std::nullopt,
            py::arg("justify_content") = std::nullopt);

        style.def_property("color", &StyleBlock::color, &StyleBlock::set_color);
        style.def_property("spacing", &StyleBlock::spacing, &StyleBlock::set_spacing);
        style.def_property("padding", &StyleBlock::padding, &StyleBlock::set_padding);
        style.def_property("x", &StyleBlock::x, &StyleBlock::set_x);
        style.def_property("y", &StyleBlock::y, &StyleBlock::set_y);
        style.def_property("width", &StyleBlock::width, &StyleBlock::set_width);
        style.def_property("height", &StyleBlock::height, &StyleBlock::set_height);
        style.def_property("visible", &StyleBlock::visible, &StyleBlock::set_visible);
        style.def_property("direction", &StyleBlock::direction, &StyleBlock::set_direction);
        style.def_property("align_items", &StyleBlock::align_items, &StyleBlock::set_align_items);
        style.def_property("justify_content", &StyleBlock::justify_content, &StyleBlock::set_justify_content);
    }

}