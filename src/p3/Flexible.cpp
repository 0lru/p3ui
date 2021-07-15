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

#include "Flexible.h"

#include <numeric>
#include <optional>
#include <iostream>

#include <imgui.h>

namespace p3
{


    Flexible::Flexible()
        : Node("Flexible") 
    {
    }

    void Flexible::update_content()
    {
        if (style_computation().direction == Direction::Vertical)
        {
            if (!style_computation().height_basis())
            {
                _automatic_height = 0.0f;
                auto first = true;
                for (auto& child : children())
                {
                    if (!child->visible())
                        continue;
                    if(!first)
                        _automatic_height += ImGui::GetStyle().ItemSpacing.y;
                    first = false;
                    _automatic_height += child->height(0);
                }
            }
            if (!style_computation().width_basis())
            {
                _automatic_width = 0.0f;
                for (auto& child : children())
                {
                    if (!child->visible())
                        continue;
                    _automatic_width = std::max(_automatic_width, child->width(0));
                }
            }
        }
        else
        {
            if (!style_computation().width_basis())
            {
                _automatic_width = 0.0f;
                auto first = true;
                for (auto& child : children())
                {
                    if (!child->visible())
                        continue;
                    if(!first)
                        _automatic_width += ImGui::GetStyle().ItemSpacing.y;
                    first = false;
                    _automatic_width += child->width(0);
                }
            }
            if (!style_computation().height_basis())
            {
                _automatic_height = 0.0f;
                for (auto& child : children())
                {
                    if (!child->visible())
                        continue;
                    _automatic_height = std::max(_automatic_height, child->height(0));
                }
            }        
        }
    }

    void Flexible::render_impl(float w, float h)
    {
        
        auto initial_cursor = ImGui::GetCursorPos();
        auto cursor = initial_cursor;
        if (style_computation().direction == Direction::Horizontal)
        {
            auto content = w;
            auto occupied = 0.0f;
            auto grow_total = 0.0f;
            bool first = true;
            std::size_t visible_count = 0;
            for (auto const& child : children())
            {
                if (!child->visible())
                    continue;
                ++visible_count;
                if (!first)
                    occupied += ImGui::GetStyle().ItemSpacing.x;
                first = false;
                //
                // fallback to 0.0, although this should be the natively computed size
                occupied += child->width(w);
                grow_total += child->style_computation().width_grow();
            }
            auto remaining = content - occupied;
            first = true;
            for (auto& child : children())
            {
                if (!child->visible())
                    continue;
                //
                // fallback to 0.0, although this should be the natively computed size
                float width = child->width(content);
                float height;
                if (remaining >= 0. && child->style_computation().width_grow() != 0.f)
                    width += remaining * (child->style_computation().width_grow() / grow_total);
                else if(remaining <0.f && child->style_computation().width_shrink() != 0.f)
                    width -= std::max(.1f, remaining * (child->style_computation().width_shrink() / grow_total));
                if (!first)
                    ImGui::SameLine();
                std::optional<float> y;
                switch (style_computation().align_items)
                {
                case Alignment::Stretch:
                    height = h;
                    break;
                case Alignment::Center:
                    height = child->height(h);
                    y = (h - height) / 2.0f;
                    break;
                case Alignment::Baseline:
                    height = child->height(h);
                    ImGui::AlignTextToFramePadding();
                    break;
                case Alignment::Start:
                    height = child->height(h);
                    y = 0.0f;
                    break;
                case Alignment::End:
                    height = child->height(h);
                    y = h - height;
                    break;
                }
                std::optional<float> x;
                if (grow_total == 0.f && remaining > 0.f) switch (style_computation().justify_content)
                {
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
                if (x) cursor.x += x.value();
                if (y) cursor.y += y.value();
                ImGui::SetCursorPos(cursor);
                child->render(width, height);
                cursor.x += width + ImGui::GetStyle().ItemSpacing.x;
                cursor.y = initial_cursor.y;
                first = false;
            }
        }
        else
        {
            auto content = h;
            auto occupied = 0.f;
            auto grow_total = 0.f;
            auto first = true;
            std::size_t visible_count = 0;
            for (auto const& child : children())
            {
                if (!child->visible())
                    continue;
                first = false;
                ++visible_count;
                occupied += child->height(content);
                grow_total += child->style_computation().height_grow();
            }
            if(visible_count > 1)
                occupied += (visible_count -1 ) * ImGui::GetStyle().ItemSpacing.y;
            auto remaining = content - occupied;
            first = true;
            for (auto& child : children())
            {
                if (!child->visible())
                    continue;
                float height = child->height(content);
                float width = 0.f;
                if (remaining >= 0.f && child->style_computation().height_grow() != 0.f)
                    height += remaining * (child->style_computation().height_grow() / grow_total);
                else if(remaining < 0.f && child->style_computation().height_shrink() != 0.f)
                    height -= std::max(0.0001f, remaining * (child->style_computation().height_shrink() / grow_total));
                std::optional<float> x;
                switch (style_computation().align_items)
                {
                case Alignment::Stretch:
                    width = w;
                    break;
                case Alignment::Center:
                    width = child->width(w);
                    x = (w - width) / 2.0f;
                    break;
                case Alignment::Start:
                    width = child->width(w);
                    x = 0.0f;
                    break;
                case Alignment::End:
                    width = child->width(w);
                    x = w - width;
                    break;
                }
                std::optional<float> y;
                if (grow_total == 0.f && remaining > 0.f) switch (style_computation().justify_content)
                {
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
                if (x) cursor.x += x.value();
                if (y) cursor.y += y.value();
                ImGui::SetCursorPos(cursor);
                child->render(width, height);
                cursor.y += height + ImGui::GetStyle().ItemSpacing.y;
                cursor.x = initial_cursor.x;
                first = false;
            }
        }
        cursor.x = initial_cursor.x + w;
        cursor.y = initial_cursor.y + h;
        ImGui::SetCursorPos(cursor);
    }

}
