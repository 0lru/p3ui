#include <catch2/catch.hpp>

#include <p3/Parser.h>

namespace p3::parser::tests
{

    TEST_CASE("skip_white_space_skips_space")
    {
        std::string data(" \n\r\t\v\f");
        auto input = data.c_str();
        REQUIRE(std::distance(input, parser::skip_whitespace(input)) == data.size());
    }

    TEST_CASE("skip_white_space_skips_comment")
    {
        std::string data("/***/");
        auto input = data.c_str();
        REQUIRE(std::distance(input, parser::skip_whitespace(input)) == data.size());
    }

    TEST_CASE("parse_casade_can_parse_inherit")
    {
        std::string data("inherit");
        auto input = data.c_str();
        Cascade cascade;
        REQUIRE(parse(input, cascade) != input);
        REQUIRE(cascade == Cascade::inherit);
    }

    TEST_CASE("parse_casaded_can_parse_initial")
    {
        std::string data("initial");
        auto input = data.c_str();
        Cascaded<Color> color;
        REQUIRE(parse(input, color) != input);
        REQUIRE(std::holds_alternative<Cascade>(color));
        REQUIRE(std::get<Cascade>(color) == Cascade::initial);
    }

    TEST_CASE("parse_casaded_can_parse_inherit")
    {
        std::string data("inherit");
        auto input = data.c_str();
        Cascaded<Color> color;
        REQUIRE(parse(input, color) != input);
        REQUIRE(std::holds_alternative<Cascade>(color));
        REQUIRE(std::get<Cascade>(color) == Cascade::inherit);
    }

    TEST_CASE("parse_casaded_can_parse_value")
    {
        std::string data("#123456");
        auto input = data.c_str();
        Cascaded<Color> color;
        REQUIRE(parse(input, color) != input);
        REQUIRE(std::holds_alternative<Color>(color));
        REQUIRE(std::get<Color>(color).value == 0x123456);
    }

    TEST_CASE("parse_length_em")
    {
        std::string data("1.3em");
        auto input = data.c_str();
        Length length;
        REQUIRE(parse(input, length) != input);
        REQUIRE(std::holds_alternative<Em>(length));
        REQUIRE(std::get<Em>(length).value == Approx(1.3f));
    }

    TEST_CASE("parse_length_must_not_contain_ws")
    {
        std::string data("1.3 em");
        auto input = data.c_str();
        Length length;
        REQUIRE(parse(input, length) == input);
    }

    TEST_CASE("parse_percentage_value")
    {
        std::string data("12.7%");
        auto input = data.c_str();
        Percentage percentage;
        REQUIRE(parse(input, percentage) != input);
        REQUIRE(percentage.value == Approx(12.7f));
    }

    TEST_CASE("cascaded_length_percentage_can_parse_em")
    {
        std::string data("123.123px");
        auto input = data.c_str();
        Cascaded<LengthPercentage> length_percentage;
        REQUIRE(parse(input, length_percentage) != input);
        REQUIRE(std::holds_alternative<LengthPercentage>(length_percentage));
        REQUIRE(std::holds_alternative<Length>(std::get<LengthPercentage>(length_percentage)));
        REQUIRE(std::holds_alternative<Px>(std::get<Length>(std::get<LengthPercentage>(length_percentage))));
    }

    TEST_CASE("array_2d_parse_length_2d")
    {
        std::string data("3px 4em");
        auto input = data.c_str();
        std::array<Length, 2> l2;
        REQUIRE(parse(input, l2) != input);
        REQUIRE(std::holds_alternative<Px>(l2[0]));
        REQUIRE(std::holds_alternative<Em>(l2[1]));
    }

    TEST_CASE("floating_point")
    {
        std::string data("1.2e2");
        auto input = data.c_str();
        float value;
        REQUIRE(parse(input, value) != input);
        REQUIRE(value == Approx(1.2e2f));
    }

    TEST_CASE("floating_point_2")
    {
        std::string data("1.2e2 3.4e5");
        auto input = data.c_str();
        std::array<float, 2> value;
        REQUIRE(parse(input, value) != input);
        REQUIRE(value[0] == Approx(1.2e2f));
        REQUIRE(value[1] == Approx(3.4e5f));
    }

    TEST_CASE("can_parse_optional_length_percentage")
    {
        {
            std::string data("auto");
            auto input = data.c_str();
            OptionalLengthPercentage olp;
            REQUIRE(parse(input, olp) != input);
        }
        {
            std::string data("100%");
            auto input = data.c_str();
            OptionalLengthPercentage olp;
            REQUIRE(parse(input, olp) != input);
        }
    }

    TEST_CASE("can_parse_flexible_length")
    {
        {
            std::string data("auto 1 1");
            auto input = data.c_str();
            LayoutLength flexible_length;
            REQUIRE(parse(input, flexible_length) != input);
        }
        {
            std::string data("33% 1 1");
            auto input = data.c_str();
            LayoutLength flexible_length;
            REQUIRE(parse(input, flexible_length) != input);
        }
        {
            std::string data("0.3px 1 1");
            auto input = data.c_str();
            LayoutLength flexible_length;
            REQUIRE(parse(input, flexible_length) != input);
        }
    }

}
