
#pragma once

#include "OnScopeExit.h"
#include "RenderBackend.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace p3 {

class Context;

class Texture {
public:
    class Observer {
    public:
        virtual ~Observer() = default;
        virtual void on_texture_resized() = 0;
    };

    Texture(std::size_t width, std::size_t height);
    ~Texture();

    void add_observer(Observer&);
    void remove_observer(Observer&);

    void resize(std::size_t width, std::size_t height);

    std::size_t width() const;
    std::size_t height() const;

    std::uint8_t* data();

    bool empty() const;

    // request update of the hardware memory
    void update();

    RenderBackend::TextureId use(Context&);

private:
    std::vector<Observer*> _observer;
    std::size_t _width;
    std::size_t _height;
    std::unique_ptr<std::uint8_t[]> _data;
    bool _updated = true;
    std::optional<RenderBackend::Texture*> _texture = std::nullopt;
    std::optional<OnScopeExit> _on_exit = std::nullopt;
};

}
