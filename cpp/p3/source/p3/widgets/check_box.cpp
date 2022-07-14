#include "check_box.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3 {

namespace {
    class LocalStyleStrategy : public StyleStrategy {
    public:
        LayoutLength const& initial_height() override
        {
            static auto initial = LayoutLength { std::nullopt, 0.f, 1.f };
            return initial;
        }
    };
    LocalStyleStrategy _style_strategy;
}

StyleStrategy& CheckBox::style_strategy() const
{
    return _style_strategy;
}

CheckBox::CheckBox(std::optional<std::string> label)
    : Node("CheckBox")
{
    set_label(std::move(label));
}

void CheckBox::render_impl(Context& context, float width, float height)
{
    ImVec2 size(width, height);
    bool value = _value;
    bool changed;
    if (_radio)
        changed = ImGui::RadioButton(imgui_label().c_str(), &value);
    else
        changed = ImGui::Checkbox(imgui_label().c_str(), &value);
    if (changed && !disabled()) {
        _value = value;
        if (_on_change) {
            postpone([f = _on_change, value = value]() {
                f(value);
            });
        }
    }
    update_status();
    render_absolute(context);
}

void CheckBox::set_on_change(OnChange on_change)
{
    _on_change = on_change;
}

CheckBox::OnChange CheckBox::on_change() const
{
    return _on_change;
}

bool CheckBox::value() const
{
    return _value;
}

void CheckBox::set_value(bool value)
{
    _value = value;
}

void CheckBox::set_radio(bool radio)
{
    _radio = radio;
    set_needs_update();
}

bool CheckBox::radio() const
{
    return _radio;
}

void CheckBox::update_content()
{

    auto const context_ptr = ImGui::GetCurrentContext();
    auto const font_size = context_ptr->FontSize;
    auto const frame_padding = context_ptr->Style.FramePadding;
    _automatic_width = _automatic_height = font_size + 2 * frame_padding.y;
    if (label()) {
        const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
        _automatic_width += label_size.x + context_ptr->Style.ItemInnerSpacing.x;
    }
}

void CheckBox::dispose()
{
    Node::dispose();
}

}