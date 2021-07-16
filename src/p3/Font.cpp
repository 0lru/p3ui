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

#include "Font.h"
#include "UserInterface.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    Font::Font(std::shared_ptr<UserInterface> user_interface, ImFont* font)
        : _user_interface(std::move(user_interface))
        , _font(font)
    {
    }

    Font::operator bool() const
    {
        return _font != nullptr;
    }

    float Font::scale() const
    {
        return _font->Scale;
    }

    UserInterface const& Font::user_interface() const
    {
        return *_user_interface;
    }

    ImFont& Font::native() const
    {
        return *_font;
    }

    float Font::size() const
    {
        return _font->FontSize;
    }

    FontAtlas::FontAtlas(std::shared_ptr<UserInterface> user_interface, ImFontAtlas* atlas)
        : _user_interface(std::move(user_interface))
        , _atlas(atlas)
    {
    }

    std::size_t FontAtlas::size() const
    {
        return static_cast<std::size_t>(_user_interface->im_gui_context().IO.Fonts->Fonts.Size);
    }

    Font FontAtlas::operator[](std::size_t index) const
    {
        return Font(_user_interface, _user_interface->im_gui_context().IO.Fonts->Fonts[static_cast<int>(index)]);
    }

    ImFontAtlas& FontAtlas::native() const
    {
        return *_atlas;
    }

}
