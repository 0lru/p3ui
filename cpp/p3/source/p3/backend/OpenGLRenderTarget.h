
#include <p3/RenderBackend.h>

namespace p3
{

    class OpenGLRenderTarget : public RenderBackend::RenderTarget
    {
    public:
        OpenGLRenderTarget(RenderBackend&, std::uint32_t width, std::uint32_t height);
        ~OpenGLRenderTarget();

        unsigned int framebuffer_id() const override;
        RenderBackend::TextureId texture_id() const override;
        void bind() override;
        void release() override;
        std::uint32_t width() const override;
        std::uint32_t height() const override;
    
        sk_sp<SkSurface> const& skia_surface() const final override;

    private:
        void _make_skia_surface(RenderBackend&);

        std::uint32_t _width;
        std::uint32_t _height;
        unsigned int _framebuffer_id;
        RenderBackend::TextureId _texture_id;
        unsigned int _stencil_id;

        sk_sp<SkSurface> _skia_surface;
    };

}
