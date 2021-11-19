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
#include "StyleDerivation.h"
#include "StyleBlock.h"

namespace p3
{

    void StyleDerivation::merge(StyleBlock& block)
    {
        if (block.position()) position = block.position().value();
        if (block.color()) color = block.color().value();
        if (block.border_width()) border_width = block.border_width().value();
        if (block.border_radius()) border_radius = block.border_radius().value();
        if (block.item_spacing()) item_spacing = block.item_spacing().value();
        if (block.spacing()) spacing = block.spacing().value();
        if (block.padding()) padding = block.padding().value();
        if (block.border_color()) border_color = block.border_color().value();
        if (block.border_shadow_color()) border_shadow_color = block.border_shadow_color().value();
        if (block.background_color()) background_color = block.background_color().value();
        if (block.visible()) visible = block.visible().value();
        if (block.x()) x = block.x().value();
        if (block.y()) y = block.y().value();
        if (block.width()) width = block.width().value();
        if (block.height()) height = block.height().value();
        if (block.direction()) direction = block.direction().value();
        if (block.justify_content()) justify_content = block.justify_content().value();
        if (block.align_items()) align_items = block.align_items().value();
    }

}
