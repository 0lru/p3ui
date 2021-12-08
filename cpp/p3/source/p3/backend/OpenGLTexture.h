
#include <p3/RenderBackend.h>


namespace p3
{

    class OpenGLTexture : public RenderBackend::Texture
    {
    public:
        OpenGLTexture();
        ~OpenGLTexture();

        RenderBackend::TextureId id() const override;

        void update(
            std::size_t width, 
            std::size_t height, 
            const std::uint8_t *rgba_data) override;
    
    private:
        RenderBackend::TextureId _id;
        unsigned int _depth_id;
    };

}
