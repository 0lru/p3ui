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

    void MenuBar::render_impl(float width, float height)
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
