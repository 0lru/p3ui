#include "button.h"
#include "../convert.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3 {

Button::Button(std::optional<std::string> label)
    : Node("Button")
{
    set_height(LayoutLength { std::nullopt, 0.f, 0.f });
    set_label(std::move(label));
}

void Button::dispose()
{
    Node::dispose();
}

void Button::render_impl(Context& context, float width, float height)
{
    ImVec2 size(width, height);
    if (_background_color) {
        ImVec4 background_color = convert(_background_color.value());
        ImGui::PushStyleColor(ImGuiCol_Button, background_color);
    }
    if (ImGui::Button(imgui_label().c_str(), size)
        && _on_click
        && !disabled()) {
        postpone([f = _on_click]() { f(); });
    }
    if (_background_color)
        ImGui::PopStyleColor();

    update_status();
    render_absolute(context);
}

void Button::set_on_click(OnClick on_click)
{
    _on_click = on_click;
}

Button::OnClick Button::on_click() const
{
    return _on_click;
}

std::optional<Color> const& Button::background_color() const
{
    return _background_color;
}

void Button::set_background_color(std::optional<Color> background_color)
{
    _background_color = std::move(background_color);
}

void Button::update_content()
{
    auto const& context = *ImGui::GetCurrentContext();
    auto const padding = context.Style.FramePadding;
    _automatic_width = _automatic_height = context.FontSize
        + 2 * padding.y
        + context.Style.FrameBorderSize;
    if (label()) {
        const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
        _automatic_width = label_size.x
            + padding.x * 2.0f
            + context.Style.FrameBorderSize;
    }
}

}
