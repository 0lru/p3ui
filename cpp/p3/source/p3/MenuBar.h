
#pragma once

#include "Node.h"

namespace p3
{
   
    class Menu;

    class MenuBar : public Node
    {
    public:
        MenuBar();

        void update_content() override;
        void render_impl(Context&, float width, float height) override;

        std::shared_ptr<MenuBar> add(std::shared_ptr<Menu>);
        void remove(std::shared_ptr<Menu> const&);

        std::vector<std::shared_ptr<Menu>> const& children() const;

    private:
        std::vector<std::shared_ptr<Menu>> _children;
    };

}
