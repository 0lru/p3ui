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
        glGenFramebuffers(1, &_framebuffer_id);
        glGenTextures(1, &reinterpret_cast<GLuint&>(_texture_id));
        glGenRenderbuffers(1, &_depth_id);

        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer_id);
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint const&>(_texture_id));

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            _width, _height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
            reinterpret_cast<GLuint const&>(_texture_id), 0);

        glBindRenderbuffer(GL_RENDERBUFFER, _depth_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_id);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("failed to create render target, framebuffer incomplete");
        }
        log_info("render target created ({}x{})", _width, _height);
    }

    OpenGLRenderTarget::~OpenGLRenderTarget()
    {
        glDeleteFramebuffers(1, &_framebuffer_id);
        glDeleteTextures(1, &reinterpret_cast<GLuint&>(_texture_id));
        glDeleteRenderbuffers(1, &_depth_id);
    }

    unsigned int OpenGLRenderTarget::framebuffer_id() const
    {
        return _framebuffer_id;
    }

    TextureId OpenGLRenderTarget::texture_id() const
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
