#include <gtest/gtest.h>

#include <p3/Factory.h>
#include <p3/Node.h>

namespace p3::tests
{

    TEST(TestFactory, can_create_button)
    {
        ASSERT_TRUE(Factory::parse("<Button/>") != nullptr);
    }

    TEST(TestFactory, can_set_node_label_attribute)
    {
        auto node = Factory::parse(R"(<Button label="abc"/>)");
        ASSERT_TRUE(node != nullptr);
        ASSERT_EQ(node->label(), "abc");
    }

    TEST(TestFactory, can_set_width_attribute_of_node_style)
    {
        auto node = Factory::parse(R"(<Button width="1px 2 3"/>)");
        ASSERT_TRUE(node != nullptr);
        ASSERT_TRUE(node->style()->width());
        ASSERT_TRUE(std::holds_alternative<FlexibleLength>(node->style()->width().value()));
        auto& length = std::get<FlexibleLength>(node->style()->width().value());
        ASSERT_TRUE(std::get<0>(length));
        ASSERT_TRUE(std::holds_alternative<Length>(std::get<0>(length).value()));
        ASSERT_EQ(std::get<1>(length), 2);
        ASSERT_EQ(std::get<2>(length), 3);
    }

    TEST(TestFactory, can_add_button_to_flexible)
    {
        auto node = Factory::parse(R"(<Flexible><Button width="1px 2 3"/></Flexible>)");
        ASSERT_EQ(node->element_name(), "Flexible");
        ASSERT_EQ(node->children().size(), 1);
        ASSERT_EQ(node->children()[0]->element_name(), "Button");
    }

}
