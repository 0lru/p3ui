#pragma once

#include "Cascadable.h"

namespace p3
{

    //
    // the computed, resp. cascaded style.
    struct StyleComputation
    {
        Position position;

        Color color;
        Length border_width;
        Length border_radius;
        Length2 item_spacing;
        Length2 spacing;
        Length2 padding;
        Color border_color;
        Color border_shadow_color;
        Color background_color;

        bool visible;
        LengthPercentage x;
        LengthPercentage y;
        LayoutLength width;
        LayoutLength height;

        Direction direction;
        Justification justify_content;
        Alignment align_items;

        OptionalLengthPercentage const& width_basis() const;
        float width_grow() const;
        float width_shrink() const;
        OptionalLengthPercentage const& height_basis() const;
        float height_grow() const;
        float height_shrink() const;
    };

}
