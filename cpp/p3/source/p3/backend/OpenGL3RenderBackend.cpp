#include <imgui.h>
#include <backends/imgui_impl_OpenGL3.h>
#include <glad/gl.h>
#include <p3/log.h>
#include "OpenGL3RenderBackend.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLTexture.h"

namespace p3
{

    void OpenGL3RenderBackend::init()
    {
        ImGui_ImplOpenGL3_Init();
        log_debug("creating skia context");
        if (!_skia_context)
            _skia_context = GrContext::MakeGL();
    }

    void OpenGL3RenderBackend::new_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void OpenGL3RenderBackend::render(UserInterface const&)
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    RenderBackend::Texture *OpenGL3RenderBackend::create_texture()
    {
        _textures.push_back(std::make_unique<OpenGLTexture>());
        return _textures.back().get();
    }
    
    RenderBackend::RenderTarget *OpenGL3RenderBackend::create_render_target(std::uint32_t width, std::uint32_t height)
    {
        _render_targets.push_back(std::make_unique<OpenGLRenderTarget>(*this, width, height));
        return _render_targets.back().get();
    }

    std::uint32_t OpenGL3RenderBackend::max_texture_size() const
    {
        GLint value;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
        return static_cast<std::uint32_t>(value);
    }

}

