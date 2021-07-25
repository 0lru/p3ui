#include <gtest/gtest.h>

#include <p3/Loader.h>
#include <p3/Node.h>

namespace p3::tests
{

    TEST(TestLoader, can_create_button)
    {
        ASSERT_TRUE(Loader::load("<Button/>") != nullptr);
    }

    TEST(TestLoader, can_set_node_label_attribute)
    {
        auto node = Loader::load(R"(<Button label="abc"/>)");
        ASSERT_TRUE(node != nullptr);
        ASSERT_EQ(node->label(), "abc");
    }

    TEST(TestLoader, can_set_width_attribute_of_node_style)
    {
        auto node = Loader::load(R"(<Button width="1px 2 3"/>)");
        ASSERT_TRUE(node != nullptr);
        ASSERT_TRUE(node->style()->width());
        ASSERT_TRUE(std::holds_alternative<FlexibleLength>(node->style()->width().value()));
        auto& length = std::get<FlexibleLength>(node->style()->width().value());
        ASSERT_TRUE(std::get<0>(length));
        ASSERT_TRUE(std::holds_alternative<Length>(std::get<0>(length).value()));
        ASSERT_EQ(std::get<1>(length), 2);
        ASSERT_EQ(std::get<2>(length), 3);
    }


}
