#pragma once

#include "Color.h"
#include "StyleTypes.h"

#include <variant>

namespace p3
{

    enum Cascade
    {
        Initial,
        Inherit
    };

    template<typename T>
    using Cascadable = std::variant<Cascade, T>;

}
