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

#include <string>
#include <functional>

#include "Node.h"
#include "Texture.h"

namespace p3
{

    class Image 
        : public Node
        , public Texture::Observer
    {
    public:
        using Callback = std::function<void()>;
        using OnClick = std::function<void()>;

        Image();
        ~Image();

        StyleStrategy& style_strategy() const override;
        void render_impl(Context&, float width, float height) override;
        void update_content() override;

        void set_texture(std::shared_ptr<Texture>);
        std::shared_ptr<Texture> texture() const;

        void set_scale(double);
        double scale() const;

        void set_on_click(OnClick);
        OnClick on_click() const;

        void on_texture_resized() override;

        virtual void synchronize_with(Synchronizable&) override;

    private:
        std::shared_ptr<Texture> _texture = nullptr;
        double _scale = 1.;
        OnClick _on_click;
    };

}
