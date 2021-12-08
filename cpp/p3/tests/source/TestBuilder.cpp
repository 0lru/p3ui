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
