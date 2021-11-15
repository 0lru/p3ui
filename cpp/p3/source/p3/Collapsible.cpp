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

#include "Collapsible.h"
#include "constant.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    Collapsible::Collapsible(std::string title)
        : Node("Collapsible")
    {
        set_label(std::move(title));
    }

    void Collapsible::render_impl(Context& context, float width, float height)
    {
        if (_force_open)
        {
            ImGui::SetNextTreeNodeOpen(_force_open.value());
            _force_open.reset();
        }
        auto window = ImGui::GetCurrentWindow();

        auto pos = ImGui::GetCursorPos();
        auto collapsed = !ImGui::CollapsingHeader(imgui_label().c_str());
        if (collapsed != _collapsed)
        {
            if (_content)
            {
                postpone([content = _content, collapsed]() {
                    content->set_visible(!collapsed);
                });
            }
            _collapsed = collapsed;
        }
        if (!_automatic_width || !_automatic_height)
            return;
        update_status();
        if (!_collapsed && _content)
        {
            auto const context_ptr = ImGui::GetCurrentContext();
            auto const font_size = context_ptr->FontSize;
            auto const frame_padding = context_ptr->Style.FramePadding;
            pos.y += _child_offset;
            ImGui::SetCursorPos(pos);
            _content->render(
                context,
                width,
                height - (font_size + 2 * frame_padding.y) - ImGui::GetStyle().ItemSpacing.y);
        }
    }

    void Collapsible::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            Node::remove(_content);
        _content = content;
        if (_content)
            Node::add(_content);
    }

    std::shared_ptr<Node> Collapsible::content()
    {
        return _content;
    }

    void Collapsible::set_collapsed(bool collapsed)
    {
        _force_open = !collapsed;
    }

    bool Collapsible::collapsed() const
    {
        return _force_open ? !_force_open : _collapsed;
    }

    void Collapsible::update_content()
    {
        auto const& context = *ImGui::GetCurrentContext();
        auto const font_size = context.FontSize;
        auto const frame_padding = context.Style.FramePadding;
        _automatic_width = 5 * font_size;
        _automatic_height = font_size + 2 * frame_padding.y;
        _child_offset = _automatic_height + context.Style.ItemSpacing.y;
        if (_content && !this->collapsed())
        {
            _automatic_width = std::max(_automatic_width, _content->automatic_width());
            _automatic_height += _content->height(0) + context.Style.ItemSpacing.y;
        }
    }

}
