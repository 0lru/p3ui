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
#include "StyleBlock.h"

namespace p3
{

    void StyleBlock::add_observer(Observer* observer)
    {
        _observer.push_back(observer);
    }

    void StyleBlock::remove_observer(Observer* observer)
    {
        _observer.erase(std::remove_if(_observer.begin(), _observer.end(), [&](auto item) {
            return item == observer;
        }), _observer.end());
    }

    void StyleBlock::_on_change()
    {
        for (auto observer : _observer)
            observer->on_style_changed();
    }

    StyleBlock::Item<Color> const& StyleBlock::color() const { return _color; }
    void StyleBlock::set_color(Item<Color> color) { _color = std::move(color);  }
    StyleBlock::Item<Length> const& StyleBlock::border_width() const { return _border_width; };
    void StyleBlock::set_border_width(Item<Length> border_width) { _border_width = std::move(border_width); _on_change(); };
    StyleBlock::Item<Length> const& StyleBlock::border_radius() const { return _border_radius; };
    void StyleBlock::set_border_radius(Item<Length> border_radius) { _border_radius = std::move(border_radius); _on_change(); };
    StyleBlock::Item<Length2> const& StyleBlock::spacing() const { return _spacing; };
    void StyleBlock::set_spacing(Item<Length2> spacing) { _spacing = std::move(spacing); _on_change(); };
    StyleBlock::Item<Length2> const& StyleBlock::item_spacing() const { return _item_spacing; };
    void StyleBlock::set_item_spacing(Item<Length2> item_spacing) { _item_spacing = std::move(item_spacing); _on_change(); };
    StyleBlock::Item<Length2> const& StyleBlock::padding() const { return _padding; };
    void StyleBlock::set_padding(Item<Length2> padding) { _padding = std::move(padding); _on_change(); };
    StyleBlock::Item<Color> const& StyleBlock::border_color() const { return _border_color; };
    void StyleBlock::set_border_color(Item<Color> border_color) { _border_color = std::move(border_color); _on_change(); };
    StyleBlock::Item<Color> const& StyleBlock::border_shadow_color() const { return _border_shadow_color; };
    void StyleBlock::set_border_shadow_color(Item<Color> border_shadow_color) { _border_shadow_color = std::move(border_shadow_color); _on_change(); };

    StyleBlock::Item<Color> const& StyleBlock::background_color() const { return _background_color; };
    void StyleBlock::set_background_color(Item<Color> background_color) { _background_color = std::move(background_color); _on_change(); };
    StyleBlock::Item<LengthPercentage> const& StyleBlock::x() const { return _x; };
    void StyleBlock::set_x(Item<LengthPercentage> x) { _x = std::move(x); _on_change(); };
    StyleBlock::Item<LengthPercentage> const& StyleBlock::y() const { return _y; };
    void StyleBlock::set_y(Item<LengthPercentage> y) { _y = std::move(y); _on_change(); };
    StyleBlock::Item<FlexibleLength> const& StyleBlock::width() const { return _width; };
    void StyleBlock::set_width(Item<FlexibleLength> width) { _width = std::move(width); _on_change(); };
    StyleBlock::Item<FlexibleLength> const& StyleBlock::height() const { return _height; };
    void StyleBlock::set_height(Item<FlexibleLength> height) { _height = std::move(height); _on_change(); };

    StyleBlock::Item<bool> const& StyleBlock::visible() const { return _visible; };
    void StyleBlock::set_visible(Item<bool> visible) { _visible = std::move(visible); _on_change(); };
    StyleBlock::Item<Direction> const& StyleBlock::direction() const { return _direction; };
    void StyleBlock::set_direction(Item<Direction> direction) { _direction = std::move(direction); _on_change(); };
    StyleBlock::Item<Justification> const& StyleBlock::justify_content() const { return _justify_content; };
    void StyleBlock::set_justify_content(Item<Justification> justify_content) { _justify_content = std::move(justify_content); _on_change(); };
    StyleBlock::Item<Alignment> const& StyleBlock::align_items() const { return _align_items; };
    void StyleBlock::set_align_items(Item<Alignment> align_items) { _align_items = std::move(align_items); _on_change(); };

}
