#include "OpenGL2RenderBackend.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl2.h>

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

}

