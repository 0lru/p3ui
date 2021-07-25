#include <gtest/gtest.h>

#include <p3/Parser.h>

namespace p3::parser::tests
{

    TEST(TestParser, skip_white_space_skips_space)
    {
        std::string data(" \n\r\t\v\f");
        auto input = data.c_str();
        ASSERT_EQ(std::distance(input, parser::skip_whitespace(input)), data.size());
    }

    TEST(TestParser, skip_white_space_skips_comment)
    {
        std::string data("/***/");
        auto input = data.c_str();
        ASSERT_EQ(std::distance(input, parser::skip_whitespace(input)), data.size());
    }

    TEST(TestParser, parse_casade_can_parse_inherit)
    {
        std::string data("inherit");
        auto input = data.c_str();
        Cascade cascade;
        ASSERT_NE(parse(input, cascade), input);
        ASSERT_EQ(cascade, Cascade::inherit);
    }

    TEST(TestParser, parse_casaded_can_parse_initial)
    {
        std::string data("initial");
        auto input = data.c_str();
        Cascaded<Color> color;
        ASSERT_NE(parse(input, color), input);
        ASSERT_TRUE(std::holds_alternative<Cascade>(color));
        ASSERT_EQ(std::get<Cascade>(color), Cascade::initial);
    }

    TEST(TestParser, parse_casaded_can_parse_inherit)
    {
        std::string data("inherit");
        auto input = data.c_str();
        Cascaded<Color> color;
        ASSERT_NE(parse(input, color), input);
        ASSERT_TRUE(std::holds_alternative<Cascade>(color));
        ASSERT_EQ(std::get<Cascade>(color), Cascade::inherit);
    }

    TEST(TestParser, parse_casaded_can_parse_value)
    {
        std::string data("#123456");
        auto input = data.c_str();
        Cascaded<Color> color;
        ASSERT_NE(parse(input, color), input);
        ASSERT_TRUE(std::holds_alternative<Color>(color));
        ASSERT_EQ(std::get<Color>(color).value, 0x123456);
    }

    TEST(TestParser, parse_length_em)
    {
        std::string data("1.3em");
        auto input = data.c_str();
        Length length;
        ASSERT_NE(parse(input, length), input);
        ASSERT_TRUE(std::holds_alternative<Em>(length));
        ASSERT_FLOAT_EQ(std::get<Em>(length).value, 1.3f);
    }

    TEST(TestParser, parse_length_must_not_contain_ws)
    {
        std::string data("1.3 em");
        auto input = data.c_str();
        Length length;
        ASSERT_EQ(parse(input, length), input);
    }

    TEST(TestParser, parse_percentage_value)
    {
        std::string data("12.7%");
        auto input = data.c_str();
        Percentage percentage;
        ASSERT_NE(parse(input, percentage), input);
        ASSERT_FLOAT_EQ(percentage.value, 12.7f);
    }

    TEST(TestParser, cascaded_length_percentage_can_parse_em)
    {
        std::string data("123.123px");
        auto input = data.c_str();
        Cascaded<LengthPercentage> length_percentage;
        ASSERT_NE(parse(input, length_percentage), input);
        ASSERT_TRUE(std::holds_alternative<LengthPercentage>(length_percentage));
        ASSERT_TRUE(std::holds_alternative<Length>(std::get<LengthPercentage>(length_percentage)));
        ASSERT_TRUE(std::holds_alternative<Px>(std::get<Length>(std::get<LengthPercentage>(length_percentage))));
    }

    TEST(TestParser, array_2d_parse_length_2d)
    {
        std::string data("3px 4em");
        auto input = data.c_str();
        std::array<Length, 2> l2;
        ASSERT_NE(parse(input, l2), input);
        ASSERT_TRUE(std::holds_alternative<Px>(l2[0]));
        ASSERT_TRUE(std::holds_alternative<Em>(l2[1]));
    }

    TEST(TestParser, floating_point)
    {
        std::string data("1.2e2");
        auto input = data.c_str();
        float value;
        ASSERT_NE(parse(input, value), input);
        ASSERT_FLOAT_EQ(value, 1.2e2f);
    }

    TEST(TestParser, floating_point_2)
    {
        std::string data("1.2e2 3.4e5");
        auto input = data.c_str();
        std::array<float, 2> value;
        ASSERT_NE(parse(input, value), input);
        ASSERT_FLOAT_EQ(value[0], 1.2e2f);
        ASSERT_FLOAT_EQ(value[1], 3.4e5f);
    }

    TEST(TestParser, can_parse_optional_length_percentage)
    {
        {
            std::string data("auto");
            auto input = data.c_str();
            OptionalLengthPercentage olp;
            ASSERT_NE(parse(input, olp), input);
        }
        {
            std::string data("100%");
            auto input = data.c_str();
            OptionalLengthPercentage olp;
            ASSERT_NE(parse(input, olp), input);
        }
    }
}
