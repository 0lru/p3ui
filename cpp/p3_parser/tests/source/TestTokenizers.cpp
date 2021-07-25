#include <gtest/gtest.h>

#include <p3/Parser.h>

namespace p3::parser::tests
{

    TEST(TestTokenizers, name_must_not_start_with_underscore)
    {
        std::string data(R"()");
        auto input = data.data();
        ASSERT_EQ(tokenizer::name(input), input);
    }

    TEST(TestTokenizers, name_can_parse_alpha_string)
    {
        std::string data(R"(abc)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::name(input)), data);
    }

    TEST(TestTokenizers, name_can_contain_underscore)
    {
        std::string data(R"(a_bc)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::name(input)), data);
    }

    TEST(TestTokenizers, floating_point_parse_leading_dot)
    {
        std::string data(R"(.0)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::floating_point(input)), data);
    }
    
    TEST(TestTokenizers, floating_point_parse_ending_dot)
    {
        std::string data(R"(0.)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::floating_point(input)), data);
    }

    TEST(TestTokenizers, floating_point_parse_dot_inbetween)
    {
        std::string data(R"(123.456)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::floating_point(input)), data);
    }

    TEST(TestTokenizers, floating_point_parse_exp)
    {
        std::string data(R"(123.456e10)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::floating_point(input)), data);
    }

    TEST(TestTokenizers, floating_point_foo_test)
    {
        std::string data(R"()");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::floating_point(input)), data);
    }

    TEST(TestTokenizers, float_point_fails_on_false_input)
    {
        std::string data(R"(12e.12)");
        auto input = data.c_str();
        ASSERT_NE(std::string(input, tokenizer::floating_point(input)), data);
    }

    TEST(TestTokenizers, comment_simple)
    {
        std::string data(R"(/*a*/)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::comment(input)), data);
    }

    TEST(TestTokenizers, comment_with_star)
    {
        std::string data(R"(/* *a*/)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::comment(input)), data);
    }

    TEST(TestTokenizers, hex_color_on_empty_string)
    {
        std::string data(R"()");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_fails_for_non_hex_color)
    {
        std::string data(R"(#abg)");
        auto input = data.c_str();
        ASSERT_NE(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_can_parse_3_digits)
    {
        std::string data(R"(#1aB)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_cannot_parse_4_digits)
    {
        std::string data(R"(#1aBa)");
        auto input = data.c_str();
        ASSERT_NE(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_cannot_parse_5_digits)
    {
        std::string data(R"(#1aBaa)");
        auto input = data.c_str();
        ASSERT_NE(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_can_parse_6_digits)
    {
        std::string data(R"(#1aBaaa)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_cannot_parse_7_digits)
    {
        std::string data(R"(#1aBaaab)");
        auto input = data.c_str();
        ASSERT_NE(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_can_parse_8_digits)
    {
        std::string data(R"(#12345678)");
        auto input = data.c_str();
        ASSERT_EQ(std::string(input, tokenizer::hex_color(input)), data);
    }

    TEST(TestTokenizers, hex_color_cannot_parse_9_digits)
    {
        std::string data(R"(#1aBaaabab)");
        auto input = data.c_str();
        ASSERT_NE(std::string(input, tokenizer::hex_color(input)), data);
    }

}
