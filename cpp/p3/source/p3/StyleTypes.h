#pragma once

#include <p3/Types.h>

namespace p3 {

using Position = p3::parser::Position;
using Px = p3::parser::Px;
using Percentage = p3::parser::Percentage;
using Em = p3::parser::Em;
using Rem = p3::parser::Rem;
using Length = p3::parser::Length;

using Length2 = std::array<Length, 2>;
using LengthPercentage = p3::parser::LengthPercentage;
using OptionalLengthPercentage = p3::parser::OptionalLengthPercentage;

using LayoutLength = p3::parser::LayoutLength;
using Direction = p3::parser::Direction;
using Alignment = p3::parser::Alignment;
using Justification = p3::parser::Justification;

template <typename T>
struct UnitType {
};

inline static UnitType<Px> const px;
inline static UnitType<Em> const em;
inline static UnitType<Rem> const rem;
inline static UnitType<Percentage> const percent;
inline static UnitType<std::nullopt_t> const auto_;

template <typename T>
Length operator|(T value, UnitType<Px> const&)
{
    return Px { static_cast<float>(value) };
}

template <typename T>
LengthPercentage operator|(T value, UnitType<Percentage> const&)
{
    return Percentage { static_cast<float>(value) };
}

template <typename T>
Length operator|(T value, UnitType<Em> const&)
{
    return Em { static_cast<float>(value) };
}

template <typename T>
Length operator|(T value, UnitType<Rem> const&)
{
    return Rem { static_cast<float>(value) };
}

}
