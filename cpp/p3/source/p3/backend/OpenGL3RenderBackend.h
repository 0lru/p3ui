#pragma once
#include <p3/RenderBackend.h>

namespace p3
{

    class OpenGL3RenderBackend final : public RenderBackend
    {
    public:
        void init() override;
        void new_frame() override;
        void render(UserInterface const&) override;

        TextureId create_texture() override;
        void delete_texture(TextureId) override;
        void update_texture(
            TextureId, 
            std::size_t width, 
            std::size_t height, 
            const std::uint8_t *data) override;
    };

}
