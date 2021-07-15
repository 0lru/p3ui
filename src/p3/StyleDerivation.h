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

namespace p3
{

    class StyleBlock;

    struct StyleDerivation
    {
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
        Cascadable<FlexibleLength> width = Cascade::Initial;
        Cascadable<FlexibleLength> height = Cascade::Initial;

        Cascadable<Direction> direction = Cascade::Initial;
        Cascadable<Justification> justify_content = Cascade::Initial;
        Cascadable<Alignment> align_items = Cascade::Initial;

        void merge(StyleBlock&);
    };

}
