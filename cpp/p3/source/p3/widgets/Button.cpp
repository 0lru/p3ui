#include "Button.h"
#include "../convert.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3 {

namespace {

    class LocalStyleStrategy : public StyleStrategy {
    public:
        LayoutLength const& initial_height() override
        {
            static auto initial = LayoutLength { std::nullopt, 0.f, 0.f };
            return initial;
        }
    };
    LocalStyleStrategy _style_strategy;

}

StyleStrategy& Button::style_strategy() const
{
    return _style_strategy;
}

Button::Button(std::optional<std::string> label)
    : Node("Button")
{
    set_label(std::move(label));
}

void Button::set_attribute(std::string const& name, std::string const& value)
{
    static auto setter = std::unordered_map<std::string, std::function<void(Node&, std::string const&)>> {};
    auto it = setter.find(name);
    if (it != setter.end())
        it->second(*this, value);
    Node::set_attribute(name, value);
}

void Button::dispose()
{
    _on_click = nullptr;
    Node::dispose();
}

void Button::render_impl(Context& context, float width, float height)
{
    bool has_color = style() && style()->background_color() && std::holds_alternative<Color>(style()->background_color().value());
        
    if (has_color) {
        auto color = std::get<Color>(style()->background_color().value());
        ImVec4 imgui_color;
        assign(imgui_color, color);
        ImGui::PushStyleColor(ImGuiCol_Button, imgui_color);
    }
    ImVec2 size(width, height);
    if (ImGui::Button(imgui_label().c_str(), size)
        && _on_click
        && !disabled()) {
        postpone([f = _on_click]() { f(); });
    }
    update_status();
    if (has_color)
        ImGui::PopStyleColor();
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
