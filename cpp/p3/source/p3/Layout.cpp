
#include "Layout.h"

#include <numeric>
#include <optional>

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

namespace {
bool _debug = false;
}

namespace p3 {

Layout::Layout()
    : Node("Layout")
{
}

void Layout::update_content()
{
    if (style_computation().direction == Direction::Vertical) {
        if (!style_computation().height_basis()) {
            _automatic_height = 0.0f;
            auto first = true;
            for (auto& child : children()) {
                if (!child->visible() || child->style_computation().position == Position::Absolute)
                    continue;
                if (!first)
                    _automatic_height += ImGui::GetStyle().ItemSpacing.y;
                first = false;
                _automatic_height += child->contextual_height(0);
            }
            _automatic_height += ImGui::GetStyle().FramePadding.y * 2.f;
        }
        if (!style_computation().width_basis()) {
            _automatic_width = 0.0f;
            for (auto& child : children()) {
                if (!child->visible() || child->style_computation().position == Position::Absolute)
                    continue;
                _automatic_width = std::max(_automatic_width, child->contextual_width(0));
            }
            _automatic_width += ImGui::GetStyle().FramePadding.x * 2.f;
        }
    } else {
        if (!style_computation().width_basis()) {
            _automatic_width = 0.0f;
            auto first = true;
            for (auto& child : children()) {
                if (!child->visible() || child->style_computation().position == Position::Absolute)
                    continue;
                if (!first)
                    _automatic_width += ImGui::GetStyle().ItemSpacing.y;
                first = false;
                _automatic_width += child->contextual_width(0);
            }
            _automatic_width += ImGui::GetStyle().FramePadding.x * 2.f;
        }
        if (!style_computation().height_basis()) {
            _automatic_height = 0.0f;
            for (auto& child : children()) {
                if (!child->visible() || child->style_computation().position == Position::Absolute)
                    continue;
                _automatic_height = std::max(_automatic_height, child->contextual_height(0));
            }
            _automatic_height += ImGui::GetStyle().FramePadding.y * 2.f;
        }
    }
}

void Layout::render_impl(Context& context, float w, float h)
{
    auto initial_cursor = ImGui::GetCursorPos();
    auto p1 = initial_cursor;

    auto const& frame_padding = ImGui::GetStyle().FramePadding;
    initial_cursor.x += frame_padding.x;
    w -= frame_padding.x * 2.f;
    initial_cursor.y += frame_padding.y;
    h -= frame_padding.y * 2.f;

    ImGui::SetCursorPos(initial_cursor);

    auto cursor = initial_cursor;
    if (style_computation().direction == Direction::Horizontal) {
        auto content = w;
        auto occupied = 0.0f;
        auto grow_total = 0.0f;
        bool first = true;
        std::size_t visible_count = 0;
        for (auto const& child : children()) {
            if (!child->visible() || child->style_computation().position == Position::Absolute)
                continue;
            ++visible_count;
            if (!first)
                occupied += ImGui::GetStyle().ItemSpacing.x;
            first = false;
            //
            // fallback to 0.0, although this should be the natively computed size
            occupied += child->contextual_width(w);
            grow_total += child->style_computation().width_grow();
        }
        auto remaining = content - occupied;
        first = true;
        for (auto& child : children()) {
            if (!child->visible() || child->style_computation().position == Position::Absolute)
                continue;
            //
            // fallback to 0.0, although this should be the natively computed size
            float width = child->contextual_width(content);
            float height;
            if (remaining >= 0. && child->style_computation().width_grow() != 0.f)
                width += remaining * (child->style_computation().width_grow() / grow_total);
            else if (remaining < 0.f && child->style_computation().width_shrink() != 0.f)
                width -= std::max(.1f, remaining * (child->style_computation().width_shrink() / grow_total));
            if (!first)
                ImGui::SameLine();
            std::optional<float> y = std::nullopt;
            switch (style_computation().align_items) {
            case Alignment::Stretch:
                height = h;
                break;
            case Alignment::Center:
                height = child->contextual_height(h);
                y = (h - height) / 2.0f;
                break;
            case Alignment::Baseline:
                height = child->contextual_height(h);
                ImGui::AlignTextToFramePadding();
                break;
            case Alignment::Start:
                height = child->contextual_height(h);
                y = 0.0f;
                break;
            case Alignment::End:
                height = child->contextual_height(h);
                y = h - height;
                break;
            }
            std::optional<float> x;
            if (grow_total == 0.f && remaining > 0.f)
                switch (style_computation().justify_content) {
                case Justification::Start:
                    x = 0.f;
                    break;
                case Justification::End:
                    x = first ? w - occupied : 0.f;
                    break;
                case Justification::SpaceAround:
                    x = remaining / (visible_count + 1);
                    break;
                case Justification::SpaceBetween:
                    x = first ? 0.f : remaining / (visible_count - 1);
                    break;
                case Justification::Center:
                    x = first ? remaining / 2.f : 0.f;
                    break;
                }
            if (x)
                cursor.x += x.value();
            if (y)
                cursor.y += y.value();
            ImGui::SetCursorPos(cursor);
            auto window = ImGui::GetCurrentWindow();
            window->DC.CurrLineTextBaseOffset = 0;
            window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y;
            float backup = 0.f;
            child->render(context, width, height, true);
            cursor.x += width + ImGui::GetStyle().ItemSpacing.x;
            cursor.y = initial_cursor.y;
            first = false;
        }
    } else {
        auto content = h;

        auto occupied = 0.f;
        auto grow_total = 0.f;
        auto first = true;
        std::size_t visible_count = 0;
        for (auto const& child : children()) {
            if (!child->visible() || child->style_computation().position == Position::Absolute)
                continue;
            first = false;
            ++visible_count;
            occupied += child->contextual_height(content);
            grow_total += child->style_computation().height_grow();
        }
        if (visible_count > 1)
            occupied += (visible_count - 1) * ImGui::GetStyle().ItemSpacing.y;
        auto remaining = content - occupied;
        first = true;
        for (auto& child : children()) {
            if (!child->visible() || child->style_computation().position == Position::Absolute)
                continue;
            float height = child->contextual_height(content);
            float width = 0.f;
            if (remaining >= 0.f && child->style_computation().height_grow() != 0.f)
                height += remaining * (child->style_computation().height_grow() / grow_total);
            else if (remaining < 0.f && child->style_computation().height_shrink() != 0.f)
                height -= std::max(0.0001f, remaining * (child->style_computation().height_shrink() / grow_total));
            std::optional<float> x;
            switch (style_computation().align_items) {
            case Alignment::Stretch:
                width = w;
                break;
            case Alignment::Center:
                width = child->contextual_width(w);
                x = (w - width) / 2.0f;
                break;
            case Alignment::Start:
                width = child->contextual_width(w);
                x = 0.0f;
                break;
            case Alignment::End:
                width = child->contextual_width(w);
                x = w - width;
                break;
            }
            std::optional<float> y;
            if (grow_total == 0.f && remaining > 0.f)
                switch (style_computation().justify_content) {
                case Justification::Start:
                    y = 0.f;
                    break;
                case Justification::End:
                    y = first ? h - occupied : 0.f;
                    break;
                case Justification::SpaceAround:
                    y = remaining / (visible_count + 1);
                    break;
                case Justification::SpaceBetween:
                    y = first ? 0.f : remaining / (visible_count - 1);
                    break;
                case Justification::Center:
                    y = first ? remaining / 2.f : 0.f;
                    break;
                }
            if (x)
                cursor.x += x.value();
            if (y)
                cursor.y += y.value();
            ImGui::SetCursorPos(cursor);
            auto window = ImGui::GetCurrentWindow();
            window->DC.CurrLineTextBaseOffset = 0;
            float backup = 0.f;
            child->render(context, width, height, true);
            cursor.y += height + ImGui::GetStyle().ItemSpacing.y;
            cursor.x = initial_cursor.x;
            first = false;
        }
    }
    cursor.x = initial_cursor.x + w + frame_padding.x;
    cursor.y = initial_cursor.y + h + frame_padding.y;
    ImGui::SetCursorPos(cursor);

    if (_debug) {
        static auto redf = ImVec4(1, 0, 0, 1);
        static auto redu = ImGui::GetColorU32(redf);
        auto& window = *ImGui::GetCurrentWindow();
        p1.x += window.Pos.x - window.Scroll.x;
        p1.y += window.Pos.y - window.Scroll.y;
        ImVec2 p2 {
            cursor.x + window.Pos.x - window.Scroll.x,
            cursor.y + window.Pos.y - window.Scroll.y
        };
        ImGui::GetWindowDrawList()->AddRect(p1, p2, redu);
    }
    render_absolute(context);
}

Direction const& Layout::direction() const { return _direction; };

void Layout::set_direction(Direction direction)
{
    _direction = std::move(direction);
    set_needs_update();
};

Justification const& Layout::justify_content() const 
{ 
    return _justify_content; 
}

void Layout::set_justify_content(Justification justify_content)
{
    _justify_content = std::move(justify_content);
    set_needs_update();
}

Alignment const& Layout::align_items() const 
{ 
    return _align_items; 
}

void Layout::set_align_items(Alignment align_items)
{
    _align_items = std::move(align_items);
    set_needs_update();
}

Length2 const& Layout::spacing() const 
{ 
    return _spacing; 
}

void Layout::set_spacing(Length2 spacing)
{
    _spacing = std::move(spacing);
    set_needs_update();
}

Length2 const& Layout::padding() const 
{ 
    return _padding; 
}

void Layout::set_padding(Length2 padding)
{
    _padding = std::move(padding);
    set_needs_update();
}

}
