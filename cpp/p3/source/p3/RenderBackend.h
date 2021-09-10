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
