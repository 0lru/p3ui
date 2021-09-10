/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#include <p3/RenderBackend.h>


namespace p3
{

    class OpenGLRenderTarget : public RenderTarget
    {
    public:
        OpenGLRenderTarget(std::uint32_t width, std::uint32_t height);
        ~OpenGLRenderTarget();

        unsigned int framebuffer_id() const override;
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
