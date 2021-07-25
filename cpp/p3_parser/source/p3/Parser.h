#include <array>
#include <string>
#include <variant>
#include <optional>
#include <stdexcept>

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

    struct Color { std::uint32_t value; };
    struct Px { float value; };
    struct Em { float value; };
    struct Rem { float value; };
    struct Percentage { float value; };
    using Length = std::variant<Px, Em, Rem>;
    using LengthPercentage = std::variant<Length, Percentage>;
    using OptionalLengthPercentage = std::optional<LengthPercentage>;
    using FlexibleLengthPercentage = std::tuple<OptionalLengthPercentage, float, float>;

    enum class Cascade { inherit, initial };

    template<typename T>
    using Cascaded = std::variant<Cascade, T>;

    pos skip_whitespace(pos);

    template<typename T>
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
            throw ParseError("failed to parse");
        return value;
    }

    template<typename T>
    struct Rule<Cascaded<T>>
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
    struct Rule<std::array<T, 2>>
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
