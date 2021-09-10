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

#include <p3/Builder.h>
#include <p3/Node.h>

namespace p3::tests
{

    TEST_CASE("can_create_button", "[p3]")
    {
        REQUIRE(Builder().build("<Button/>") != nullptr);
    }

    TEST_CASE("can_set_node_label_attribute", "")
    {
        auto node = Builder().build(R"(<Button label="abc"/>)");
        REQUIRE(node != nullptr);
        REQUIRE(node->label() == "abc");
    }

    TEST_CASE("can_set_width_attribute_of_node_style", "")
    {
        auto node = Builder().build(R"(<Button width="1px 2 3"/>)");
        REQUIRE(node != nullptr);
        REQUIRE(node->style()->width());
        REQUIRE(std::holds_alternative<LayoutLength>(node->style()->width().value()));
        auto& length = std::get<LayoutLength>(node->style()->width().value());
        REQUIRE(std::get<0>(length));
        REQUIRE(std::holds_alternative<Length>(std::get<0>(length).value()));
        REQUIRE(std::get<1>(length) == 2);
        REQUIRE(std::get<2>(length) == 3);
    }

    TEST_CASE("can_add_button_to_flexible", "")
    {
        auto node = Builder().build(R"(<Layout><Button width="1px 2 3"/></Layout>)");
        REQUIRE(node->element_name() == "Layout");
        REQUIRE(node->children().size() == 1);
        REQUIRE(node->children()[0]->element_name() == "Button");
    }

}
