#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace p3
{

    using TextureId = void*;
    class UserInterface;

    //
    // abstract render-backend
    class RenderBackend
    {
    public:
        virtual ~RenderBackend() = default;
        virtual void init() = 0;
        virtual void new_frame() = 0;
        virtual void render(UserInterface const&) = 0;
    };

}
