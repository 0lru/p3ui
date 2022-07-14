#include "ToolTip.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3 {

ToolTip::ToolTip()
    : Node("ToolTip")
{
    set_position(Position::Absolute);
}

void ToolTip::render(Context& context, float width, float height, bool)
{
    if (!_content)
        return;
    if (!ImGui::IsItemHovered())
        return;
    float alpha = .95f;

    push_style();
    auto& style = ImGui::GetCurrentContext()->Style;
    std::swap(alpha, style.Alpha);
    ImGui::BeginTooltip();
    auto avail = ImGui::GetContentRegionAvail();
    _content->render(context,
        _content->contextual_width(avail.x),
        _content->contextual_height(avail.y));
    ImGui::EndTooltip();
    std::swap(alpha, style.Alpha);
    pop_style();
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
    _automatic_width = _content->contextual_minimum_content_width() + 2 * style.FramePadding.x + style.FrameBorderSize;
    _automatic_height = _content->contextual_minimum_content_height() + 2 * style.FramePadding.y + style.FrameBorderSize;
}

}
