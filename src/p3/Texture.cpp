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

#include "Texture.h"

#include <iostream>

namespace p3
{

    Texture::Texture(std::size_t width, std::size_t height)
    {
        resize(width, height);
        glGenTextures(1, &_id);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &_id);
    }

    bool Texture::empty() const
    {
        return _width * _height == 0;
    }

    GLuint Texture::id() const
    {
        return _id;
    }

    void Texture::update()
    {
        if (empty())
            return;
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLsizei(width()), GLsizei(height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, data());
    }

    void Texture::resize(std::size_t width, std::size_t height)
    {
        _width = width;
        _height = height;
        auto pixels = _width * _height;
        if (pixels > 0)
            _data = std::make_unique<std::uint8_t[]>(pixels * 4);
        else
            _data.reset();
    }

    std::size_t Texture::width() const
    {
        return _width;
    }

    std::size_t Texture::height() const
    {
        return _height;
    }

    std::uint8_t* Texture::data()
    {
        return _data.get();
    }

}
