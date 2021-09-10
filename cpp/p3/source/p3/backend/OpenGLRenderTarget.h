
#include <p3/RenderBackend.h>


namespace p3
{

    class OpenGLRenderTarget : public RenderTarget
    {
    public:
        OpenGLRenderTarget(std::uint32_t width, std::uint32_t height);
        ~OpenGLRenderTarget();

        TextureId texture_id() const override;
        void bind() override;
        void release() override;
        std::uint32_t width() const override;
        std::uint32_t height() const override;
    
    private:
        std::uint32_t _width;
        std::uint32_t _height;
        unsigned int _framebuffer_id;
        TextureId _texture_id;
        unsigned int _depth_id;
    };

}
