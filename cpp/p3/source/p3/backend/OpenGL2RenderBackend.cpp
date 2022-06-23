#include <imgui.h>
#include <backends/imgui_impl_opengl2.h>
#include <glad/gl.h>

#include "OpenGL2RenderBackend.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLTexture.h"

namespace p3
{

    void OpenGL2RenderBackend::init()
    {
        ImGui_ImplOpenGL2_Init();
        if (!_skia_context)
            _skia_context = GrContext::MakeGL();
    }

    void OpenGL2RenderBackend::new_frame()
    {
        ImGui_ImplOpenGL2_NewFrame();
    }

    void OpenGL2RenderBackend::render(UserInterface const&)
    {
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    RenderBackend::Texture* OpenGL2RenderBackend::create_texture()
    {
        _textures.push_back(std::make_unique<OpenGLTexture>());
        return _textures.back().get();
    }

    std::uint32_t OpenGL2RenderBackend::max_texture_size() const
    {
        GLint value;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
        return static_cast<std::uint32_t>(value);
    }

    RenderBackend::RenderTarget* OpenGL2RenderBackend::create_render_target(std::uint32_t width, std::uint32_t height)
    {
        _render_targets.push_back(std::make_unique<OpenGLRenderTarget>(*this, width, height));
        return _render_targets.back().get();
    }

}
