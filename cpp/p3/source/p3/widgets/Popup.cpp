#include "Popup.h"

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

namespace p3 {

namespace {
    class LocalStyleStrategy : public StyleStrategy {
    public:
        LayoutLength const& initial_width() override
        {
            static auto initial = LayoutLength { std::nullopt, 0.f, 0.f };
            return initial;
        }
        LayoutLength const& initial_height() override
        {
            static auto initial = LayoutLength { std::nullopt, 0.f, 0.f };
            return initial;
        }
    };
    LocalStyleStrategy _style_strategy;
}

StyleStrategy& Popup::style_strategy() const
{
    return _style_strategy;
}

Popup::Popup()
    : Node("Popup")
{
}

void Popup::render_impl(Context& context, float width, float height)
{
    if (!_opened) {
        ImGui::OpenPopup(imgui_label().c_str());
        _opened = true;
    }
    if (ImGui::BeginPopup(imgui_label().c_str())) {
        _content->render(context, this->contextual_width(0), this->contextual_height(0));
        ImGui::EndPopup();
    } else {
        _opened = false;
        if (_on_close)
            _on_close();
    }
    update_status();
}

void Popup::update_content()
{
    _content->update_content();
    _automatic_width = _content->contextual_minimum_content_width();
    _automatic_height = _content->contextual_minimum_content_height();
}

void Popup::set_content(std::shared_ptr<Node> content)
{
    _content = content;
}

std::shared_ptr<Node> Popup::content() const
{
    return _content;
}

void Popup::set_on_close(OnClose on_close)
{
    _on_close = on_close;
}

Popup::OnClose Popup::on_close() const
{
    return _on_close;
}

bool Popup::opened() const
{
    return _opened;
}

}
