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

#include "ToolTip.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

    ToolTip::ToolTip()
        : Node("ToolTip")
    {
        style()->set_position(Position::Absolute);
    }

    void ToolTip::render(Context& context, float width, float height, bool)
    {
        auto compiled_guard = _apply_style_compiled();
        if (!_content)
            return;
        if (!ImGui::IsItemHovered())
            return;
        ImGui::BeginTooltip();
        auto avail = ImGui::GetContentRegionAvail();
        auto& style = ImGui::GetCurrentContext()->Style;
        _content->render(context, 
            _content->width(avail.x), 
            _content->height(avail.y));
        ImGui::EndTooltip();
    }

    void ToolTip::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            Node::remove(_content);
        _content = content;
        if (_content)
            Node::add(_content);
    }

    std::shared_ptr<Node> ToolTip::content() const
    {
        return _content;
    }

    void ToolTip::update_content()
    {
        auto& style = ImGui::GetCurrentContext()->Style;
        _automatic_width = _content->automatic_width() + 2 * style.FramePadding.x + style.FrameBorderSize;
        _automatic_height = _content->automatic_height() + 2 * style.FramePadding.y + style.FrameBorderSize;
    }

}
