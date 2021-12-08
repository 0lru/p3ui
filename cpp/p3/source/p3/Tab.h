
#pragma once

#include <string>
#include <functional>

#include "Node.h"

namespace p3
{

    class Tab : public Node
    {
    public:
        class Item;

        Tab();
        void render_impl(Context&, float width, float height) override;
        void update_content() override;
    };

    class Tab::Item : public Node
    {
    public:
        Item(std::string name="", std::shared_ptr<Node> = nullptr);

        std::shared_ptr<Node> content() const;
        void set_content(std::shared_ptr<Node>);
        void render(Context&, float width, float height, bool) override;
        
        void update_content();

    private:
        std::shared_ptr<Node> _content;
    };

}
