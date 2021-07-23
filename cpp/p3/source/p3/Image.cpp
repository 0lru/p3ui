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

#include "Image.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

    namespace
    {
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            FlexibleLength const& initial_width() override
            {
                static auto initial = FlexibleLength{ std::nullopt, 0.f, 0.f };
                return initial;
            }
            FlexibleLength const& initial_height() override
            {
                static auto initial = FlexibleLength{ std::nullopt, 0.f, 0.f };
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& Image::style_strategy() const
    {
        return _style_strategy;
    }

    Image::Image()
        : Node("Image")
    {
    }

    void Image::render_impl(Context& context, float width, float height)
    {
        if (!_texture)
            return;
        ImVec2 size(width, height);
        auto id = reinterpret_cast<ImTextureID>(_texture->use(context));
        ImGui::Image(id, size);
        update_status();
    }

    void Image::update_content()
    {
        _automatic_height = _texture ? _texture->height() * float(_scale) : 0.f;
        _automatic_width = _texture ? _texture->width() * float(_scale) : 0.f;
    }

    void Image::set_texture(std::shared_ptr<Texture> texture)
    {
        _texture = std::move(texture);
    }

    std::shared_ptr<Texture> Image::texture() const
    {
        return _texture;
    }

    void Image::set_scale(double scale)
    {
        _scale = scale;
        set_needs_update();
    }

    double Image::scale() const
    {
        return _scale;
    }

}
