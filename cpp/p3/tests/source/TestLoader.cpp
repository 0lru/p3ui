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

}
