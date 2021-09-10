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
#pragma once
#include <p3/RenderBackend.h>

namespace p3
{

    class OpenGL2RenderBackend final : public RenderBackend
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

        std::shared_ptr<RenderTarget> create_render_target(
            std::uint32_t width, 
            std::uint32_t height) override;
        void delete_render_target(std::shared_ptr<RenderTarget>) override;
    };

}
