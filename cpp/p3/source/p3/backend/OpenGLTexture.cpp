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
#include <glad/gl.h>
#include <stdexcept>
#include <p3/log.h>

#include "OpenGLTexture.h"

namespace p3
{

    OpenGLTexture::OpenGLTexture()
    {
        glGenTextures(1, &reinterpret_cast<GLuint&>(_id));
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint&>(_id));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

        glGenTextures(1, &reinterpret_cast<GLuint&>(_id));
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &reinterpret_cast<GLuint&>(_id));
    }

    RenderBackend::TextureId OpenGLTexture::id() const
    {
        return _id;
    }

    void OpenGLTexture::update(
        std::size_t width,
        std::size_t height,
        std::uint8_t const* rgba_data)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0, GL_RGBA,
            static_cast<GLsizei>(width),
            static_cast<GLsizei>(height),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            rgba_data);
    }

}
