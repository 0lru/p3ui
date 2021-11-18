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

#include "MenuItem.h"
#include "Context.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

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
            _checkable ? &_value : nullptr, _enabled))
        {
            if (_on_click)
                postpone(_on_click);
            if (_checkable && _on_change)
                postpone([f = _on_change, value=_value]() {
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

    void MenuItem::dispose()
    {
        _on_click = nullptr;
        _on_change = nullptr;
        Node::dispose();
    }

}
