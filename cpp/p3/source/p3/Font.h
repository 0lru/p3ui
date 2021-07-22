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

#include <memory>

struct ImFont;
struct ImFontAtlas;

namespace p3
{

    class UserInterface;

    class Font
    {
    public:
        Font(std::shared_ptr<UserInterface>, ImFont*);
        operator bool() const;
        float size() const;
        float scale() const;
        UserInterface const& user_interface() const;
        ImFont& native() const;

    private:
        std::shared_ptr<UserInterface> _user_interface;
        ImFont *_font;
    };

    class FontAtlas
    {
    public:
        FontAtlas(std::shared_ptr<UserInterface>, ImFontAtlas*);

        std::size_t size() const;
        Font operator[](std::size_t) const;
        
        ImFontAtlas& native() const;

    private:
        std::shared_ptr<UserInterface> _user_interface;
        ImFontAtlas* _atlas;
    };

}