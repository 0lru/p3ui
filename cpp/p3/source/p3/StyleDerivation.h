#pragma once

#include "Cascadable.h"

namespace p3
{

    class StyleBlock;

    //
    // this is the default, cascadable styling. one or more style 
    // blocks can be merged to override these defaults.
    // e.g. this uses a lot of memory and is probably optimizable
    struct StyleDerivation
    {
        Cascadable<Position> position = Cascade::Initial;
        Cascadable<Color> color = Cascade::Inherit;
        Cascadable<Length> border_width = Cascade::Initial;
        Cascadable<Length> border_radius = Cascade::Initial;
        Cascadable<Length2> item_spacing = Cascade::Initial;
        Cascadable<Length2> spacing = Cascade::Initial;
        Cascadable<Length2> padding = Cascade::Initial;
        Cascadable<Color> border_color = Cascade::Initial;
        Cascadable<Color> border_shadow_color = Cascade::Initial;
        Cascadable<Color> background_color = Cascade::Initial;

        Cascadable<bool> visible = Cascade::Initial;
        Cascadable<LengthPercentage> x = Cascade::Initial;
        Cascadable<LengthPercentage> y = Cascade::Initial;
        Cascadable<LayoutLength> width = Cascade::Initial;
        Cascadable<LayoutLength> height = Cascade::Initial;

        Cascadable<Direction> direction = Cascade::Initial;
        Cascadable<Justification> justify_content = Cascade::Initial;
        Cascadable<Alignment> align_items = Cascade::Initial;

        void merge(StyleBlock&);
    };

}
