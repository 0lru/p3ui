/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/
#include <catch2/catch.hpp>

#include <p3/Parser.h>

namespace p3::parser::tests
{

    TEST_CASE("name_must_not_start_with_underscore")
    {
        std::string data(R"()");
        auto input = data.data();
        REQUIRE(tokenizer::name(input) == input);
    }

    TEST_CASE("name_can_parse_alpha_string")
    {
        std::string data(R"(abc)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::name(input)) == data);
    }

    TEST_CASE("can_parse_em")
    {
        std::string data(R"(em)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::em(input)) == data);
    }

    TEST_CASE("can_parse_rem")
    {
        std::string data(R"(rem)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::rem(input)) == data);
    }

    TEST_CASE("can_parse_px")
    {
        std::string data(R"(px)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::px(input)) == data);
    }

    TEST_CASE("can_parse_auto")
    {
        std::string data(R"(auto)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::auto_(input)) == data);
    }

    TEST_CASE("name_can_contain_underscore")
    {
        std::string data(R"(a_bc)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::name(input)) == data);
    }

    TEST_CASE("floating_point_parse_leading_dot")
    {
        std::string data(R"(.0)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::floating_point(input)) == data);
    }
    
    TEST_CASE("floating_point_parse_ending_dot")
    {
        std::string data(R"(0.)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::floating_point(input)) == data);
    }

    TEST_CASE("floating_point_parse_dot_inbetween")
    {
        std::string data(R"(123.456)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::floating_point(input)) == data);
    }

    TEST_CASE("floating_point_parse_exp")
    {
        std::string data(R"(123.456e10)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::floating_point(input)) == data);
    }

    TEST_CASE("floating_point_foo_test")
    {
        std::string data(R"()");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::floating_point(input)) == data);
    }

    TEST_CASE("float_point_fails_on_false_input")
    {
        std::string data(R"(12e.12)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::floating_point(input)) != data);
    }

    TEST_CASE("comment_simple")
    {
        std::string data(R"(/*a*/)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::comment(input)) == data);
    }

    TEST_CASE("comment_with_star")
    {
        std::string data(R"(/* *a*/)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::comment(input)) == data);
    }

    TEST_CASE("hex_color_on_empty_string")
    {
        std::string data(R"()");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) == data);
    }

    TEST_CASE("hex_color_fails_for_non_hex_color")
    {
        std::string data(R"(#abg)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) != data);
    }

    TEST_CASE("hex_color_can_parse_3_digits")
    {
        std::string data(R"(#1aB)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) == data);
    }

    TEST_CASE("hex_color_cannot_parse_4_digits")
    {
        std::string data(R"(#1aBa)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) != data);
    }

    TEST_CASE("hex_color_cannot_parse_5_digits")
    {
        std::string data(R"(#1aBaa)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) != data);
    }

    TEST_CASE("hex_color_can_parse_6_digits")
    {
        std::string data(R"(#1aBaaa)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) == data);
    }

    TEST_CASE("hex_color_cannot_parse_7_digits")
    {
        std::string data(R"(#1aBaaab)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) != data);
    }

    TEST_CASE("hex_color_can_parse_8_digits")
    {
        std::string data(R"(#12345678)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) == data);
    }

    TEST_CASE("hex_color_cannot_parse_9_digits")
    {
        std::string data(R"(#1aBaaabab)");
        auto input = data.c_str();
        REQUIRE(std::string(input, tokenizer::hex_color(input)) != data);
    }

}
