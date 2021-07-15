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

#include "constant.h"
#include "ScrollArea.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

    ScrollArea::ScrollArea()
        : Node("ScrollArea")
    {
    }

    void ScrollArea::render_impl(float width, float height)
    {
        ImVec2 size(width, height);
        ImGuiWindowFlags flags = 0;
        if (_horizontal_scroll_enabled)
            flags |= ImGuiWindowFlags_HorizontalScrollbar;
        if (!_horizontal_scroll_autohide)
            flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
        if (!_vertical_scroll_autohide)
            flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
        // auto content_width = _content->width(state);
        // auto content_height = _content->height(state);
        // ImGui::SetNextWindowContentSize(ImVec2(content_width, content_height));
        ImGui::BeginChild(imgui_label().c_str(), size, true, flags);
        if (_content)
        {
            auto available = ImGui::GetContentRegionAvail();
            _content->render(_content->width(available.x), _content->height(available.y));
        }
        ImGui::EndChild();
        update_status();
    }

    void ScrollArea::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            Node::remove(_content);
        _content = std::move(content);
        if (_content)
            Node::add(_content);
    }

    std::shared_ptr<Node> const& ScrollArea::content() const
    {
        return _content;
    }

    void ScrollArea::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        _automatic_width = _automatic_height = DefaultItemWidthEm * font_size;
        if (_content)
            _content->update_content();
    }

    void ScrollArea::set_horizontal_scroll_enabled(bool value)
    {
        _horizontal_scroll_enabled = value;
    }

    bool ScrollArea::horizontal_scroll_enabled() const
    {
        return _horizontal_scroll_enabled;
    }

    void ScrollArea::set_horizontal_scroll_autohide(bool value)
    {
        _horizontal_scroll_autohide = value;
    }

    bool ScrollArea::horizontal_scroll_autohide() const
    {
        return _horizontal_scroll_autohide;
    }

    void ScrollArea::set_vertical_scroll_autohide(bool value)
    {
        _vertical_scroll_autohide = value;
    }

    bool ScrollArea::vertical_scroll_autohide() const
    {
        return _vertical_scroll_autohide;
    }

}