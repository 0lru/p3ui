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
    };

}
