#include "ProgressBar.h"

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
                static auto initial = LayoutLength{std::nullopt, 0.f, 1.f};
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& ProgressBar::style_strategy() const
    {
        return _style_strategy;
    }

    ProgressBar::ProgressBar()
        : Node("ProgressBar")
    {
    }

    void ProgressBar::render_impl(Context&, float width, float height)
    {
        
        ImVec2 size(label() ? width * GoldenRatio : width, height);
        ImGui::ProgressBar(_value, size);
        if (label())
        {
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text(label().value().c_str());
        }
    }

    void ProgressBar::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2 * frame_padding.y;
        _automatic_width = DefaultItemWidthEm * font_size;
        if (label())
            _automatic_width
            += ImGui::CalcTextSize(label().value().c_str(), NULL, true).x
            + context_ptr->Style.ItemInnerSpacing.x;
    }

    void ProgressBar::set_value(float value)
    {
        _value = value;
    }

    float ProgressBar::value() const
    {
        return _value;
    }

}
