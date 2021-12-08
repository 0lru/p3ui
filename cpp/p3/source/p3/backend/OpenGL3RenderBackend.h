#pragma once
#include <vector>
#include <p3/RenderBackend.h>

namespace p3
{

    class OpenGL3RenderBackend final : public RenderBackend
    {
    public:
        void init() override;
        void new_frame() override;
        void render(UserInterface const&) override;
        
        Texture *create_texture() override;
        RenderTarget *create_render_target(std::uint32_t width, std::uint32_t height) override;
    };

}
