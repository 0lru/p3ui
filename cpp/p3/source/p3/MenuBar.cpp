
#include "MenuBar.h"
#include "Menu.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{
    MenuBar::MenuBar()
        : Node("MenuBar")
    {
    }
    
    void MenuBar::update_content()
    {
        for (auto& child : _children)
            child->update_content();
    }

    void MenuBar::render_impl(Context&, float width, float height)
    {
    }

    std::shared_ptr<MenuBar> MenuBar::add(std::shared_ptr<Menu> menu)
    {
        _children.push_back(std::move(menu));
        return std::static_pointer_cast<MenuBar>(shared_from_this());
    }

    void MenuBar::remove(std::shared_ptr<Menu> const& menu)
    {
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& ptr) {
            return ptr == menu;
        }), _children.end());
    }

    std::vector<std::shared_ptr<Menu>> const& MenuBar::children() const
    {
        return _children;
    }

}
