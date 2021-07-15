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

#include "Plot.h"

#include <iostream>

#include <imgui.h>
#include <implot.h>


namespace p3
{

    std::vector<char const*> reference_tick_labels(std::vector<std::string> const& tick_labels)
    {
        // NOTE: use ranges
        std::vector<char const*> result(tick_labels.size());
        std::transform(tick_labels.begin(), tick_labels.end(), result.begin(), [](auto const& label) {
            return label.c_str();
        });
        return result;
    }

    Plot::Plot()
        : Node("Plot")
    {
    }

    void Plot::set_x_label(Label x_label)
    {
        _x_label = std::move(x_label);
    }

    Plot::Label const& Plot::x_label() const
    {
        return _x_label;
    }

    void Plot::set_y_label(Label y_label)
    {
        _y_label = std::move(y_label);
    }

    Plot::Label const& Plot::y_label() const
    {
        return _y_label;
    }

    void Plot::set_x_limits(Limits x_limits)
    {
        _x_limits = std::move(x_limits);
    }

    Plot::Limits const& Plot::x_limits() const
    {
        return _x_limits;
    }

    void Plot::set_y_limits(Limits y_limits)
    {
        _y_limits = std::move(y_limits);
    }

    Plot::Limits const& Plot::y_limits() const
    {
        return _y_limits;
    }

    void Plot::set_x_ticks(std::optional<Ticks> x_ticks)
    {
        _x_ticks = std::move(x_ticks);
    }

    std::optional<Plot::Ticks> const& Plot::x_ticks() const
    {
        return _x_ticks;
    }

    std::optional<Plot::Ticks>& Plot::x_ticks()
    {
        return _x_ticks;
    }

    void Plot::set_y_ticks(std::optional<Ticks> y_ticks)
    {
        _y_ticks = std::move(y_ticks);
    }

    std::optional<Plot::Ticks> const& Plot::y_ticks() const
    {
        return _y_ticks;
    }

    std::optional<Plot::Ticks>& Plot::y_ticks()
    {
        return _y_ticks;
    }

    void Plot::set_x_tick_labels(std::optional<TickLabels> x_tick_labels)
    {
        _x_tick_labels = std::move(x_tick_labels);
    }

    std::optional<Plot::TickLabels> const& Plot::x_tick_labels() const
    {
        return _x_tick_labels;
    }

    void Plot::set_y_tick_labels(std::optional<TickLabels> y_tick_labels)
    {
        _y_tick_labels = std::move(y_tick_labels);
    }

    std::optional<Plot::TickLabels> const& Plot::y_tick_labels() const
    {
        return _y_tick_labels;
    }

    void Plot::render_impl(float width, float height)
    {
        ImVec2 size(width, height);

        ImPlotAxisFlags x_flags, y_flags = 0;

        if (_x_limits)
            ImPlot::SetNextPlotLimitsX(_x_limits.value()[0], _x_limits.value()[1], ImGuiCond_Always);
        else
            x_flags |= ImPlotAxisFlags_AutoFit;
        if (_y_limits)
            ImPlot::SetNextPlotLimitsY(_y_limits.value()[0], _y_limits.value()[1], ImGuiCond_Always);
        else
            y_flags |= ImPlotAxisFlags_AutoFit;

        if (_x_ticks)
        {
            if (_x_tick_labels)
            {
                auto references = reference_tick_labels(_x_tick_labels.value());
                auto count = int(std::min(_x_ticks.value().size(), _x_tick_labels.value().size()));
                ImPlot::SetNextPlotTicksX(_x_ticks.value().data(), count, references.data(), false);
            }
            else
                ImPlot::SetNextPlotTicksX(_x_ticks.value().data(), int(_x_ticks.value().size()), 0, false);
        }

        if (_y_ticks)
        {
            if (_y_tick_labels)
            {
                auto references = reference_tick_labels(_y_tick_labels.value());
                auto count = int(std::min(_y_ticks.value().size(), _y_tick_labels.value().size()));
                ImPlot::SetNextPlotTicksY(_y_ticks.value().data(), count, references.data(), false);
            }
            else
            {
                ImPlot::SetNextPlotTicksY(_y_ticks.value().data(), int(_y_ticks.value().size()), 0, false);
            }
        }

        if (ImPlot::BeginPlot(
            imgui_label().c_str(),
            _x_label ? _x_label.value().c_str() : 0,
            _y_label ? _y_label.value().c_str() : 0,
            size,
            0,
            ImPlotAxisFlags_AutoFit,
            ImPlotAxisFlags_AutoFit))
        {
            for (auto& item : _items)
                item->render();

            ImPlot::EndPlot();
        }
    }

    void Plot::add(std::shared_ptr<Item> item)
    {
        _items.push_back(std::move(item));
    }

    void Plot::remove(std::shared_ptr<Item> item)
    {
        _items.erase(std::remove_if(_items.begin(), _items.end(), [&](auto iterated) {
            return iterated == item;
        }), _items.end());
    }

    void Plot::clear()
    {
        _items.clear();
    }

    void Plot::update_content()
    {
        _automatic_height = _automatic_width = 0.f;
    }

}