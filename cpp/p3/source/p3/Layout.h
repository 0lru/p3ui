
#pragma once

#include <string>

#include "Node.h"
#include <imgui.h>

namespace p3
{

    class Layout : public Node
    {
    public:
        Layout();

        void update_content() override;
        void render_impl(Context&, float width, float height) override;
    };

}
