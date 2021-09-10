#include <imgui.h>
#include <backends/imgui_impl_OpenGL3.h>
#include <glad/gl.h>

#include "OpenGL3RenderBackend.h"
#include "OpenGLRenderTarget.h"

namespace p3
{

    void OpenGL3RenderBackend::init()
    {
        ImGui_ImplOpenGL3_Init();
    }

    void OpenGL3RenderBackend::new_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void OpenGL3RenderBackend::render(UserInterface const&)
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    TextureId OpenGL3RenderBackend::create_texture()
    {
        TextureId id;
        glGenTextures(1, &reinterpret_cast<GLuint&>(id));
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint&>(id));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        return id;
    }

    void OpenGL3RenderBackend::delete_texture(TextureId id)
    {
        glDeleteTextures(1, &reinterpret_cast<GLuint&>(id));
    }

    void OpenGL3RenderBackend::update_texture(
        TextureId,
        std::size_t width,
        std::size_t height,
        std::uint8_t const* data)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0, GL_RGBA,
            static_cast<GLsizei>(width),
            static_cast<GLsizei>(height),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data);
    }

    std::shared_ptr<RenderTarget> OpenGL3RenderBackend::create_render_target(std::uint32_t width, std::uint32_t height)
    {
        return std::make_shared<OpenGLRenderTarget>(width, height);
    }

    void OpenGL3RenderBackend::delete_render_target(std::shared_ptr<RenderTarget>)
    {
        // ...
    }




}

