#pragma once

#include <array>
#include <string>
#include <variant>
#include <optional>

namespace p3::parser
{

    struct Color { std::uint32_t value; };
    struct Px { float value; };
    struct Em { float value; };
    struct Rem { float value; };
    struct Percentage { float value; };
    using Length = std::variant<Px, Em, Rem>;
    using LengthPercentage = std::variant<Length, Percentage>;
    using OptionalLengthPercentage = std::optional<LengthPercentage>;
    using LayoutLength = std::tuple<OptionalLengthPercentage, float, float>;
    enum class Cascade { inherit, initial };
    enum class Position { Static, Absolute };
    template<typename T>
    using Cascaded = std::variant<Cascade, T>;

    enum class Direction
    {
        Horizontal=0,
        Vertical=1
    };

    enum class Alignment
    {
        Start=0,
        Center=1,
        End=2,
        Stretch=3,
        Baseline=4
    };

    enum class Justification
    {
        SpaceBetween = 0,
        SpaceAround = 1,
        Start = 2,
        Center = 3,
        End = 4
    };

}
