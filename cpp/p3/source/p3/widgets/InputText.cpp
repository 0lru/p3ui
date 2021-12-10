#include "InputText.h"

#include <p3/constant.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

    namespace
    {

        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{ std::nullopt, 0.f, 1.f };
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& InputText::style_strategy() const
    {
        return _style_strategy;
    }

    InputText::InputText(std::size_t size, std::optional<std::string> label)
        : Node("InputText")
        , _value()
        , _size(4)
    {
        _value.reserve(4);
        set_label(std::move(label));
    }

    void InputText::render_impl(Context&, float width, float height)
    {
        ImGui::SetNextItemWidth(label() ? width * GoldenRatio : width);
        if (_hint)
        {
            if (ImGui::InputTextWithHint(imgui_label().c_str(), _hint.value().c_str(), _value.data(), _size))
                if (_on_change)
                    postpone(_on_change);
        }
        else
        {
            if (ImGui::InputText( imgui_label().c_str(), _value.data(), _value.capacity(),
                ImGuiInputTextFlags_CallbackResize, InputText::Callback, this))
                if (_on_change)
                    postpone(_on_change);
        }
        update_status();
    }

    void InputText::set_hint(std::optional<std::string> hint)
    {
        _hint = std::move(hint);
    }

    std::optional<std::string> const& InputText::hint(std::string) const
    {
        return _hint;
    }

    void InputText::set_on_change(OnChange on_change)
    {
        _on_change = on_change;
    }

    InputText::OnChange InputText::on_change() const
    {
        return _on_change;
    }

    void InputText::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2 * frame_padding.y;
        _automatic_width = DefaultItemWidthEm * font_size + 2 * frame_padding.x;
        if (label())
        {
            const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
            _automatic_width += label_size.x + context_ptr->Style.ItemInnerSpacing.x;
        }
    }

    void InputText::set_value(std::string value)
    {
        _value = std::move(value);
    }

    std::string const& InputText::value() const
    {
        return _value;
    }

    int InputText::Callback(ImGuiInputTextCallbackData* data)
    {
        auto self = static_cast<InputText *>(data->UserData);
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            self->_value.resize(data->BufTextLen);
            data->Buf = self->_value.data();
        }
        return 0;
    }

    void InputText::dispose()
    {
        _on_change = nullptr;
        Node::dispose();
    }

}