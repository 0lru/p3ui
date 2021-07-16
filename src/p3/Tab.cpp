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
        ImGui::BeginTabBar((imgui_label() + "_tab").c_str());
        for (auto& child : children())
            child->render(width, height);
        ImGui::EndTabBar();
        ImGui::EndChild();
        update_status();
    }

    void Tab::update_content()
    {
        for (auto& child : children())
            child->update_content();
        _automatic_height = 1.f;
        _automatic_width = 1.f;
    }

    Tab::Item::Item(std::string name, std::shared_ptr<Node> node)
        : Node("TabItem")
        , _content(std::move(node))
    {
        set_label(name);
    }

    std::shared_ptr<Node> Tab::Item::content() const
    {
        return _content;
    }

    void Tab::Item::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            remove(_content);
        _content = std::move(content);
        if (_content)
            add(_content);
    }

    void Tab::Item::render(float width, float height)
    {
        if (!ImGui::BeginTabItem(label() ? label().value().c_str() : ""))
            return;
        auto compiled_guard = _apply_style_compiled();
        if (_content)
        {
            auto region = ImGui::GetContentRegionAvail();
            _content->render(region.x, region.y);
        }
        ImGui::EndTabItem();
    }

    void Tab::Item::update_content()
    {
        if (_content)
        {
            _content->update_content();
            _automatic_width = _content->automatic_width();
            _automatic_height = _content->automatic_height();
        }
        else
        {
            _automatic_width = _automatic_height = 0.f;
        }
    }

}
