#include "InputScalar.h"

#include <p3/constant.h>
#include <p3/imgui_datatype.h>

#include <imgui.h>
#include <imgui_internal.h>


namespace p3
{

    template<typename DataType>
    inline void InputScalar<DataType>::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2 * frame_padding.y;
        _automatic_width = DefaultItemWidthEm * font_size;
        if (label())
        {
            const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
            _automatic_width += label_size.x + context_ptr->Style.ItemInnerSpacing.x;
        }
    }

    template<typename DataType>
    inline void InputScalar<DataType>::render_impl(Context&, float width, float height)
    {
        if (label())
            ImGui::SetNextItemWidth(width * 0.65f);
        else
            ImGui::SetNextItemWidth(width);
        DataType value = _value;
        auto vptr = disabled() ? &value : &_value;
        bool changed = ImGui::InputScalar(
            imgui_label().c_str(),
            imgui_datatype<DataType>,
            vptr,
            _step ? &_step.value() : NULL,
            _format ? _format.value().c_str() : nullptr,
            0);
        if (changed && _on_change && !disabled())
            postpone([f = _on_change, value = _value]() {
                f(value);
            });
        update_status();
    }

    template InputScalar<std::int8_t>;
    template InputScalar<std::uint8_t>;
    template InputScalar<std::int16_t>;
    template InputScalar<std::uint16_t>;
    template InputScalar<std::int32_t>;
    template InputScalar<std::uint32_t>;
    template InputScalar<std::int64_t>;
    template InputScalar<std::uint64_t>;
    template InputScalar<float>;
    template InputScalar<double>;

}