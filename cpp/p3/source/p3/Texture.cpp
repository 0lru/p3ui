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

#include "Texture.h"
#include "Context.h"

#include <iostream>

namespace p3
{

    Texture::Texture(std::size_t width, std::size_t height)
    {
        resize(width, height);
    }

    Texture::~Texture()
    {
    }

    bool Texture::empty() const
    {
        return _width * _height == 0;
    }

    void Texture::update()
    {
        _updated = true;
    }

    void Texture::add_observer(Observer& observer)
    {
        _observer.push_back(&observer);
    }

    void Texture::remove_observer(Observer& observer)
    {
        std::erase(_observer, &observer);
    }

    TextureId Texture::use(Context& context)
    {
        auto& backend = context.render_backend();
        if (!_texture_id)
        {
            _texture_id = context.render_backend().create_texture();
            _on_exit = OnScopeExit([texture_id = _texture_id.value(), backend = context.render_backend().shared_from_this()](){
                backend->delete_texture(texture_id);
            });
        }
        if (_updated)
        {
            context.render_backend().update_texture(_texture_id.value(), _width, _height, _data.get());
            _updated = false;
        }
        return _texture_id.value();
    }

    void Texture::resize(std::size_t width, std::size_t height)
    {
        if (_width == width && _height == height)
            return;
        _width = width;
        _height = height;
        auto pixels = _width * _height;
        if (pixels > 0)
            _data = std::make_unique<std::uint8_t[]>(pixels * 4);
        else
            _data.reset();
        for (auto observer : _observer)
            observer->on_texture_resized();
    }

    std::size_t Texture::width() const
    {
        return _width;
    }

    std::size_t Texture::height() const
    {
        return _height;
    }

    std::uint8_t* Texture::data()
    {
        return _data.get();
    }

}
