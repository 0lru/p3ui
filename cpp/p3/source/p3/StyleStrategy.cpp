#include "StyleStrategy.h"

namespace p3
{

    LayoutLength const& StyleStrategy::initial_width()
    {
        static auto initial = LayoutLength{ std::nullopt, 1.f, 0.f };
        return initial;
    }
    
    LayoutLength const& StyleStrategy::initial_height()
    {
        static auto initial = LayoutLength{ std::nullopt, 1.f, 0.f };
        return initial;
    }
    
    Color StyleStrategy::initial_background_color()
    {
        return Color(0x00000000);
    }
    
    Color StyleStrategy::current_background_color()
    {
        return Color(0x00000000);
    }

}