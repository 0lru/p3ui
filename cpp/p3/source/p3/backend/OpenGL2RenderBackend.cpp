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

    RenderBackend::RenderTarget* OpenGL2RenderBackend::create_render_target(std::uint32_t width, std::uint32_t height)
    {
        _render_targets.push_back(std::make_unique<OpenGLRenderTarget>(width, height));
        return _render_targets.back().get();
    }

}
