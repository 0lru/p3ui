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

#include "RenderBackend.h"
#include "OnScopeExit.h"

#include <string>
#include <memory>
#include <optional>

namespace p3
{

    class Context;

    class Texture
    {
    public:
        Texture(std::size_t width, std::size_t height);
        ~Texture();

        void resize(std::size_t width, std::size_t height);

        std::size_t width() const;
        std::size_t height() const;

        std::uint8_t* data();

        bool empty() const;

        bool dirty() const;
        void set_dirty();

        TextureId use(Context&);

    private:
        std::size_t _width;
        std::size_t _height;
        std::unique_ptr<std::uint8_t[]> _data;
        bool _dirty = true;
        std::optional<TextureId> _texture_id = std::nullopt;
        std::optional<OnScopeExit> _on_exit = std::nullopt;
    };

}
