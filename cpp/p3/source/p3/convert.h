#include "Color.h"

#include <imgui.h>
#include <implot.h>

namespace p3 {

inline bool operator==(ImVec4 const& v1, ImVec4 const& v2)
{
    return v1.x == v2.x && v1.y == v2.y && v1.w == v2.w && v1.z == v2.z;
}

inline bool operator!=(ImVec4 const& v1, ImVec4 const& v2)
{
    return !(v1 == v2);
}

inline bool operator==(ImVec2 const& v1, ImVec2 const& v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}

inline bool operator!=(ImVec2 const& v1, ImVec2 const& v2)
{
    return !(v1 == v2);
}

inline void assign(ImVec4& im, Color const& color)
{
    im.x = color.red() / 255.f;
    im.y = color.green() / 255.f;
    im.z = color.blue() / 255.f;
    im.w = color.alpha() / 255.f;
}

inline void assign(Color& color, ImVec4 const& src)
{
    color = Color(
        std::uint8_t(src.x * 255.f),
        std::uint8_t(src.y * 255.f),
        std::uint8_t(src.z * 255.f),
        std::uint8_t(src.w * 255.f));
}

inline void assign(ImVec4& v, std::optional<Color> const& color)
{
    v = color
        ? ImVec4(
            color.value().red() / 255.f,
            color.value().green() / 255.f,
            color.value().blue() / 255.f,
            color.value().alpha() / 255.f)
        : IMPLOT_AUTO_COL;
}

inline void assign(std::optional<Color>& color, ImVec4 const& src)
{
    if (src == IMPLOT_AUTO_COL)
        color = std::nullopt;
    else
        color = Color(
            std::uint8_t(src.x * 255.f),
            std::uint8_t(src.y * 255.f),
            std::uint8_t(src.z * 255.f),
            std::uint8_t(src.w * 255.f));
}

inline std::optional<Color> convert(ImVec4 const& v)
{
    if (v == IMPLOT_AUTO_COL)
        return std::nullopt;
    Color color;
    assign(color, v);
    return color;
}

inline ImVec4 convert(Color const& color)
{
    ImVec4 v;
    assign(v, color);
    return v;
}

inline ImVec4 convert(std::optional<Color> const& color)
{
    return color ? convert(color.value()) : IMPLOT_AUTO_COL;
}

}