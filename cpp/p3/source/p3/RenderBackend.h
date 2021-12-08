#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <vector>

namespace p3
{

    class UserInterface;

    //
    // abstract render-backend
    class RenderBackend : public std::enable_shared_from_this<RenderBackend>
    {
    public:
        using TextureId = void*;

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

        class Texture
        {
        public:
            virtual ~Texture() = default;
            virtual TextureId id() const = 0;
            virtual void update(std::size_t width, std::size_t height, const std::uint8_t* rgba_data) = 0;
        };

        virtual ~RenderBackend() = default;
        
        virtual void init() = 0;
        virtual void new_frame() = 0;
        virtual void render(UserInterface const&) = 0;

        virtual Texture* create_texture() = 0;
        virtual RenderTarget* create_render_target(std::uint32_t width, std::uint32_t height) = 0;

        void gc();
        void shutdown();

        void exec(std::function<void()>&&);
        void delete_texture(Texture*);
        void delete_render_target(RenderTarget*);

    protected:
        std::vector<std::unique_ptr<Texture>> _textures;
        std::vector<std::unique_ptr<RenderTarget>> _render_targets;

    private:
        std::vector<Texture*> _deleted_textures;
        std::vector<RenderTarget*> _deleted_render_targets;
        std::vector<std::function<void()>> _tasks;
    };

}
