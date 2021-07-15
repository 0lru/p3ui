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

#include "Context.h"
#include "Font.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    Context::Font::Font(std::shared_ptr<Context> context, ImFont* font)
        : _context(std::move(context))
        , _font(font)
    {
    }

    Context::Font::operator bool() const
    {
        return _font != nullptr;
    }

    float Context::Font::scale() const
    {
        return _font->Scale;
    }

    float Context::Font::size() const
    {
        return _font->FontSize;
    }

    Context::FontAtlas::FontAtlas(std::shared_ptr<Context> context, ImFontAtlas* atlas)
        : _context(std::move(context))
        , _atlas(atlas)
    {
    }

    std::size_t Context::FontAtlas::size() const
    {
        return static_cast<std::size_t>(_context->_gui_context->IO.Fonts->Fonts.Size);
    }

    Context::Font Context::FontAtlas::operator[](std::size_t index) const
    {
        return Font(_context, _context->_gui_context->IO.Fonts->Fonts[static_cast<int>(index)]);
    }

}
