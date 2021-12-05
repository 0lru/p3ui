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

#include "OpenGLRenderTarget.h"

namespace p3
{

    OpenGLRenderTarget::OpenGLRenderTarget(std::uint32_t width, std::uint32_t height)
        : _width(width)
        , _height(height)
    {
        // only save texture state. framebuffer state is handled in a correct way
        glPushAttrib(GL_TEXTURE_BIT);
        glGenFramebuffers(1, &_framebuffer_id);
        glGenTextures(1, &reinterpret_cast<GLuint&>(_texture_id));
        glGenRenderbuffers(1, &_stencil_id);

        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer_id);
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint const&>(_texture_id));

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            _width, _height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
            reinterpret_cast<GLuint const&>(_texture_id), 0);

        glBindRenderbuffer(GL_RENDERBUFFER, _stencil_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, _width, _height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _stencil_id);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("failed to create render target, framebuffer incomplete");
        
        log_debug("render target created ({}x{})", _width, _height);
        glPopAttrib();
    }

    OpenGLRenderTarget::~OpenGLRenderTarget()
    {
        glDeleteFramebuffers(1, &_framebuffer_id);
        glDeleteTextures(1, &reinterpret_cast<GLuint&>(_texture_id));
        glDeleteRenderbuffers(1, &_stencil_id);
    }

    unsigned int OpenGLRenderTarget::framebuffer_id() const
    {
        return _framebuffer_id;
    }

    RenderBackend::TextureId OpenGLRenderTarget::texture_id() const
    {
        return _texture_id;
    }

    void OpenGLRenderTarget::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer_id);
        glViewport(0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
    }

    void OpenGLRenderTarget::release()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    std::uint32_t OpenGLRenderTarget::width() const
    {
        return _width;
    }

    std::uint32_t OpenGLRenderTarget::height() const
    {
        return _height;
    }

}
