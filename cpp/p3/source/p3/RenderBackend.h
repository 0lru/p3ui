#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <cstdint>

namespace p3
{

    using TextureId = void*;
    class UserInterface;

    class RenderTarget
    {
    public:
        virtual ~RenderTarget() = default;

        virtual unsigned int framebuffer_id() const = 0;
        virtual TextureId texture_id() const = 0;
        virtual void bind() = 0;
        virtual void release() = 0;
        virtual std::uint32_t width() const = 0;
        virtual std::uint32_t height() const = 0;
    };

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

        virtual std::shared_ptr<RenderTarget> create_render_target(
            std::uint32_t width, 
            std::uint32_t height) = 0;
        virtual void delete_render_target(std::shared_ptr<RenderTarget>) = 0;
    };

}
