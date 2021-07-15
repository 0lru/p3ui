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

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    Collapsible::Collapsible(std::string title)
        : Node("Collapsible")
        , _title(std::move(title))
    {
    }

    void Collapsible::render_impl(float width, float height)
    {
        ImVec2 size(width, height);
        // children alignment is ok, justification is also
        // fine, if collapsible has a well defined height
        _collapsed = !ImGui::CollapsingHeader(_title.c_str());
        if (!_automatic_width || !_automatic_height)
            return;
        update_status();
        if (!_collapsed && _content)
        {
            auto const context_ptr = ImGui::GetCurrentContext();
            auto const font_size = context_ptr->FontSize;
            auto const frame_padding = context_ptr->Style.FramePadding;
            _content->render(width, 
                height - (font_size + 2 * frame_padding.y) - ImGui::GetStyle().ItemSpacing.y);
        }
    }

    void Collapsible::set_title(std::string title)
    {
        _title = title;
    }

    std::string const& Collapsible::title() const
    {
        return _title;
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

    bool Collapsible::is_collapsed() const
    {
        return false;
    }

    void Collapsible::update_content()
    {
        // TODO: optimize this, compute only if needed
        _content->update_content();
        _automatic_width = 50.f; // TODO: compute this
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2 * frame_padding.y;
        if (_content && !_collapsed)
        {
            _automatic_width = std::max(50.f, _content->automatic_width());
            _automatic_height +=  _content->height(0) + ImGui::GetStyle().ItemSpacing.y;
        }
    }

}
