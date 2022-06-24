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
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint&>(_id));
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
