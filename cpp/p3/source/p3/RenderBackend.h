#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace p3
{

    using TextureId = void*;
    class UserInterface;

    //
    // abstract render-backend
    class RenderBackend : public std::enable_shared_from_this<RenderBackend>
    {
    public:
        virtual ~RenderBackend() = default;
        virtual void init() = 0;
        virtual void new_frame() = 0;
        virtual void render(UserInterface const&) = 0;

        virtual TextureId create_texture() = 0;
        virtual void delete_texture(TextureId) = 0;
        virtual void update_texture(
            TextureId, 
            std::size_t width, 
            std::size_t height, 
            const std::uint8_t *data) =0;
    };

}
