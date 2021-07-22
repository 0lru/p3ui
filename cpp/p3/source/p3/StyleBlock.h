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
#pragma once

#include "Cascadable.h"

#include <optional>

namespace p3
{

    class StyleBlock
    {
    public:
        class Observer
        {
        public:
            virtual ~Observer() = default;
            virtual void on_style_changed() = 0;
        };

        void add_observer(Observer*);
        void remove_observer(Observer*);

        template<typename T>
        using Item = std::optional<Cascadable<T>>;

        Item<Color> const& color() const;
        void set_color(Item<Color>);

        Item<Length> const& border_width() const;
        void set_border_width(Item<Length>);
        Item<Length> const& border_radius() const;
        void set_border_radius(Item<Length>);
        Item<Length2> const& spacing() const;
        void set_spacing(Item<Length2>);
        Item<Length2> const& item_spacing() const;
        void set_item_spacing(Item<Length2>);
        Item<Length2> const& padding() const;
        void set_padding(Item<Length2>);
        Item<Color> const& border_color() const;
        void set_border_color(Item<Color>);
        Item<Color> const& border_shadow_color() const;
        void set_border_shadow_color(Item<Color>);

        Item<Color> const& background_color() const;
        void set_background_color(Item<Color>);
        Item<LengthPercentage> const& x() const;
        void set_x(Item<LengthPercentage>);
        Item<LengthPercentage> const& y() const;
        void set_y(Item<LengthPercentage>);
        Item<FlexibleLength> const& width() const;
        void set_width(Item<FlexibleLength>);
        Item<FlexibleLength> const& height() const;
        void set_height(Item<FlexibleLength>);

        Item<bool> const& visible() const;
        void set_visible(Item<bool>);
        Item<Direction> const& direction() const;
        void set_direction(Item<Direction>);
        Item<Justification> const& justify_content() const;
        void set_justify_content(Item<Justification>);
        Item<Alignment> const& align_items() const;
        void set_align_items(Item<Alignment>);

    private:
        Item<Color> _color = std::nullopt;
        Item<Length> _border_width = std::nullopt;
        Item<Length> _border_radius = std::nullopt;
        Item<Length2> _spacing = std::nullopt;
        Item<Length2> _item_spacing = std::nullopt;
        Item<Length2> _padding = std::nullopt;
        Item<Color> _border_color = std::nullopt;
        Item<Color> _border_shadow_color = std::nullopt;

        Item<Color> _background_color = std::nullopt;
        Item<LengthPercentage> _x = std::nullopt;
        Item<LengthPercentage> _y = std::nullopt;
        Item<FlexibleLength> _width = std::nullopt;
        Item<FlexibleLength> _height = std::nullopt;

        Item<bool> _visible = std::nullopt;
        Item<Direction> _direction = std::nullopt;
        Item<Justification> _justify_content = std::nullopt;
        Item<Alignment> _align_items = std::nullopt;

        void _on_change();
        std::vector<Observer*> _observer;
    };

}
