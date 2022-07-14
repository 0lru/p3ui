#pragma once

#include <array>
#include <optional>
#include <string>
#include <variant>

namespace p3::parser {

struct Color {
    std::uint32_t value;
};
struct Px {
    float value;
};
struct Em {
    float value;
};
struct Rem {
    float value;
};
struct Percentage {
    float value;
};
using Length = std::variant<Px, Em, Rem>;
using LengthPercentage = std::variant<Length, Percentage>;
using OptionalLengthPercentage = std::optional<LengthPercentage>;
using LayoutLength = std::tuple<OptionalLengthPercentage, float, float>;
enum class Cascade { inherit,
    initial };
enum class Position { Static,
    Absolute };
template <typename T>
using Cascaded = std::variant<Cascade, T>;

inline bool operator==(Length const& l, Length const& r)
{
    if (std::holds_alternative<Px>(l) && std::holds_alternative<Px>(r))
        return std::get<Px>(l).value == std::get<Px>(r).value;
    if (std::holds_alternative<Em>(l) && std::holds_alternative<Em>(r))
        return std::get<Em>(l).value == std::get<Em>(r).value;
    if (std::holds_alternative<Rem>(l) && std::holds_alternative<Rem>(r))
        return std::get<Rem>(l).value == std::get<Rem>(r).value;
    return false;
}

inline bool operator==(LengthPercentage const& l, LengthPercentage const& r)
{
    if (std::holds_alternative<Length>(l) && std::holds_alternative<Length>(r))
        return std::get<Length>(l) == std::get<Length>(r);
    if (std::holds_alternative<Percentage>(l) && std::holds_alternative<Percentage>(r))
        return std::get<Percentage>(l) == std::get<Percentage>(r);
    return false;
}

enum class Direction {
    Horizontal = 0,
    Vertical = 1
};

enum class Alignment {
    Start = 0,
    Center = 1,
    End = 2,
    Stretch = 3,
    Baseline = 4
};

enum class Justification {
    SpaceBetween = 0,
    SpaceAround = 1,
    Start = 2,
    Center = 3,
    End = 4
};

}
