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

#include "Tab.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>

namespace p3
{

    Tab::Tab()
        : Node("Tab")
    {
    }

    void Tab::render_impl(float width, float height)
    {
        ImVec2 size(width, height);
        ImGui::BeginChild(imgui_label().c_str(), size, false, ImGuiWindowFlags_NoScrollbar);
        auto em = ImGui::GetCurrentContext()->FontSize;
        ImGui::BeginTabBar((imgui_label()+"_tab").c_str());
        for (auto& item : _items)
        {
            if (!ImGui::BeginTabItem(item->name().c_str()))
                continue;
            if (item->content())
            {
                auto available = ImGui::GetContentRegionAvail();
                item->content()->render(available.x, available.y);
            }
            ImGui::EndTabItem();
        }
        ImVec2 pad(1.6f * em, 1.6f * em);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, pad);
        ImGui::EndTabBar();
        ImGui::PopStyleVar();
        ImGui::EndChild();
        update_status();
    }

    void Tab::update_content()
    {
        for(auto& item : _items)
            if (item->content())
                item->content()->update_content();
        _automatic_height = 1.f;
        _automatic_width = 1.f;
    }

    void Tab::add(std::shared_ptr<Item> item)
    {
        if (item && item->content())
            Node::add(item->content());
        _items.push_back(std::move(item));
    }

    void Tab::remove(std::shared_ptr<Item> item)
    {
        _items.erase(std::remove_if(_items.begin(), _items.end(), [&](auto& it) {
            if (item == it)
            {
                if(it->content())
                    Node::remove(it->content());
                return true;
            }
            return false;
        }), _items.end());
    }

    Tab::Item::Item(std::string name, std::shared_ptr<Node> node)
        : _name(std::move(name))
        , _content(std::move(node))
    {
    }

    std::string const& Tab::Item::name() const
    {
        return _name;
    }

    void Tab::Item::set_name(std::string name)
    {
        _name = std::move(name);
        if (_tab)
            _tab->set_needs_update();
    }

    std::shared_ptr<Node> Tab::Item::content() const
    {
        return _content;
    }

    void Tab::Item::set_content(std::shared_ptr<Node> content)
    {
        if (_content && _tab)
        {
            static_cast<Node *>(_tab)->remove(_content);
        }
        _content = std::move(content);
        if (_content && _tab)
            static_cast<Node *>(_tab)->add(_content);
    }

    void Tab::Item::set_tab(Tab* tab)
    {
        _tab = tab;
    }

    Tab* Tab::Item::tab() const
    {
        return _tab;
    }

}
