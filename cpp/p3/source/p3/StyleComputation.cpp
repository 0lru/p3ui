#include "StyleComputation.h"

namespace p3
{

    OptionalLengthPercentage const& StyleComputation::width_basis() const
    {
        return std::get<0>(width);
    }

    float StyleComputation::width_grow() const
    {
        return std::get<1>(width);
    }

    float StyleComputation::width_shrink() const
    {
        return std::get<2>(width);
    }

    OptionalLengthPercentage const& StyleComputation::height_basis() const
    {
        return std::get<0>(height);
    }

    float StyleComputation::height_grow() const
    {
        return std::get<1>(height);
    }

    float StyleComputation::height_shrink() const
    {
        return std::get<2>(height);
    }


}
