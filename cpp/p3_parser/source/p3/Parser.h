#pragma once

#include <array>
#include <string>
#include <variant>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace p3::parser
{

    class ParseError : std::runtime_error
    {
    public:
        ParseError(std::string const& what) : std::runtime_error(what) {}
    };

    using pos = char const*;

    namespace tokenizer
    {

        pos name(pos);
        pos floating_point(pos);
        pos hex_color(pos);
        pos em(pos);
        pos rem(pos);
        pos px(pos);
        pos percent(pos);
        pos comment(pos);
        pos auto_(pos);

    }

    //
    // "ast"
    struct Color { std::uint32_t value; };
    struct Px { float value; };
    struct Em { float value; };
    struct Rem { float value; };
    struct Percentage { float value; };
    using Length = std::variant<Px, Em, Rem>;
    using LengthPercentage = std::variant<Length, Percentage>;
    using OptionalLengthPercentage = std::optional<LengthPercentage>;
    using FlexibleLength = std::tuple<OptionalLengthPercentage, float, float>;

    template<typename E>
    std::unordered_map<std::string, E> enum_table;

    enum class Cascade { inherit, initial };
    template<> inline static auto enum_table<Cascade> = std::unordered_map<std::string, Cascade>{
        {"inherit", Cascade::inherit},
        {"initial", Cascade::initial}
    };

    template<typename T>
    using Cascaded = std::variant<Cascade, T>;

    enum class Direction
    {
        Horizontal=0,
        Vertical=1
    };
    template<> inline static auto enum_table<Direction> = std::unordered_map<std::string, Direction>{
        {"horizontal", Direction::Horizontal},
        {"vertical", Direction::Vertical}
    };

    enum class Alignment
    {
        Start=0,
        Center=1,
        End=2,
        Stretch=3,
        Baseline=4
    };
    template<> inline static auto enum_table<Alignment> = std::unordered_map<std::string, Alignment>{
        {"start", Alignment::Start},
        {"center", Alignment::Center},
        {"end", Alignment::End},
        {"stretch", Alignment::Stretch},
        {"baseline", Alignment::Baseline}
    };

    enum class Justification
    {
        SpaceBetween = 0,
        SpaceAround = 1,
        Start = 2,
        Center = 3,
        End = 4
    };
    template<> inline static auto enum_table<Justification> = std::unordered_map<std::string, Justification>{
        {"space_between", Justification::SpaceBetween},
        {"space_around", Justification::SpaceAround},
        {"start", Justification::Start},
        {"center", Justification::Center},
        {"end", Justification::End}
    };

    //
    // parser
    pos skip_whitespace(pos);

    template<typename T, typename = void >
    struct Rule { static pos parse(pos, T&); };

    template<typename T>
    pos parse(pos begin, T& t)
    {
        return Rule<T>::parse(begin, t);
    }

    template<typename T>
    T parse(pos begin)
    {
        T value;
        auto it = parse(begin, value);
        if (it == begin)
            throw ParseError("todo: include line/char info");
        return value;
    }

    //
    // spec.

    template<typename T>
    struct Rule<T, typename std::enable_if< std::is_enum<T>::value >::type>
    {
        inline static pos parse(pos begin, T& t)
        {
            auto it = skip_whitespace(begin);
            if (auto temp = tokenizer::name(it); temp != it)
            {
                t = enum_table<T>[std::string(it, temp)];
                return temp;
            }
            return begin;
        }
    };

    template<typename T>
    struct Rule<Cascaded<T>, void>
    {
        inline static pos parse(pos begin, Cascaded<T>& ast)
        {
            auto it = skip_whitespace(begin);
            T t;
            if (auto temp = Rule<T>::parse(begin, t); it != temp)
            {
                ast = t;
                return temp;
            }
            Cascade c;
            if (auto temp = Rule<Cascade>::parse(begin, c); it != temp)
            {
                ast = c;
                return temp;
            }
            return begin;
        }
    };

    template<typename T>
    struct Rule<std::array<T, 2>, void>
    {
        inline static pos parse(pos begin, std::array<T, 2>& value)
        {
            auto it = skip_whitespace(begin);
            if (auto temp = parser::parse(it, value[0]); temp != it)
                it = temp;
            else
                return begin;
            if (auto temp = skip_whitespace(it); temp == it)
                return begin;
            else
                it = temp;
            if (auto temp = parser::parse(it, value[1]); temp != it)
                return temp;
            else
                return begin;
        }
    };

}
