
#include "Texture.h"
#include "Context.h"

#include <iostream>

namespace p3 {

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
    // 20 std::erase(_observer, &observer);
    _observer.erase(std::remove(_observer.begin(), _observer.end(), &observer), _observer.end());
}

RenderBackend::TextureId Texture::use(Context& context)
{
    auto& backend = context.render_backend();
    if (!_texture) {
        _texture = context.render_backend().create_texture();
        _on_exit = OnScopeExit([texture = _texture.value(), backend = context.render_backend().shared_from_this()]() {
            backend->delete_texture(texture);
        });
    }
    if (_updated) {
        _texture.value()->update(_width, _height, _data.get());
        _updated = false;
    }
    return _texture.value()->id();
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
