#pragma once

#include "Cascadable.h"

namespace p3
{

    class StyleStrategy
    {
    public:
        virtual ~StyleStrategy() = default;
        virtual LayoutLength const& initial_width();
        virtual LayoutLength const& initial_height();
        virtual Color initial_background_color();
        virtual Color current_background_color();
    };

}
