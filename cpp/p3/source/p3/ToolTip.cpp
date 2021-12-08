
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
