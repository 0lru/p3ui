#include "MenuItem.h"

#include <p3/Context.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace p3 {

MenuItem::MenuItem(std::string label)
    : Node("MenuItem")
{
    set_label(std::move(label));
}

void MenuItem::update_content()
{
    _automatic_width = _automatic_height = 0.f;
}

void MenuItem::render_impl(Context&, float width, float height)
{
    if (ImGui::MenuItem(imgui_label().c_str(),
            _shortcut ? _shortcut.value().c_str() : nullptr,
            _checkable ? &_value : nullptr, _enabled)) {
        if (_on_click)
            postpone(_on_click);
        if (_checkable && _on_change)
            postpone([f = _on_change, value = _value]() {
                f(value);
            });
    }
    update_status();
}

void MenuItem::set_enabled(bool enabled)
{
    _enabled = enabled;
}

bool MenuItem::enabled() const
{
    return _enabled;
}

void MenuItem::set_shortcut(std::optional<std::string> shortcut)
{
    _shortcut = std::move(shortcut);
}

std::optional<std::string> const& MenuItem::shortcut() const
{
    return _shortcut;
}

void MenuItem::set_value(bool value)
{
    _value = value;
}

bool MenuItem::value() const
{
    return _value;
}

void MenuItem::set_checkable(bool checkable)
{
    _checkable = checkable;
}

bool MenuItem::checkable() const
{
    return _checkable;
}

void MenuItem::set_on_click(OnClick on_click)
{
    _on_click = on_click;
}

MenuItem::OnClick MenuItem::on_click() const
{
    return _on_click;
}

void MenuItem::set_on_change(OnChange on_change)
{
    _on_change = on_change;
}

MenuItem::OnChange MenuItem::on_change() const
{
    return _on_change;
}

}
