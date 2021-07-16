#include <cstdint>
#include <memory>
#include <string>

namespace p3ui
{

    using TextureId = void*;

    //
    // the "backend" of a UserInterface instance
    class Viewport : public std::enable_shared_from_this<Viewport>
    {
    public:
        virtual ~Viewport() = default;

        virtual std::string viewport_info() const = 0;
        virtual double dpi() const = 0;

        virtual std::size_t width() const = 0;
        virtual std::size_t height() const = 0;
        
        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;
        
        // RGBA8 only
        virtual TextureId create_texture() = 0;
        virtual void set_texture_data(std::size_t width, std::size_t height, std::uint8_t const *) = 0;
        virtual void delete_texture(TextureId) = 0;
    };

    class UserInterface;

    //
    // abstract render-backend
    class RenderBackend
    {
    public:
        virtual ~RenderBackend() = default;
        virtual void init() = 0;
        virtual void shutdown() = 0;
        virtual void begin_frame() = 0;
        virtual void render(UserInterface const&) = 0;
    };

    class OpenGL2RenderBackend final : public RenderBackend
    {
    public:

    };

}
