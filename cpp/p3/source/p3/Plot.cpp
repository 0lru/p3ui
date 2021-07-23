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
        , _x_axis(std::make_shared<Axis>())
        , _y_axis(std::make_shared<Axis>())
    {
        // 
        // for styling..
        Node::add(_x_axis);
        Node::add(_y_axis);
    }

    void Plot::render_impl(Context&, float width, float height)
    {
        ImVec2 size(width, height);

        ImPlotAxisFlags x_flags = 0;
        if (x_axis()->type() != Axis::Type::Numeric)
        {
            if (x_axis()->type() == Axis::Type::Logarithmic)
                x_flags |= ImPlotAxisFlags_LogScale;
            else
            {
                x_flags |= ImPlotAxisFlags_Time;
                ImPlot::GetStyle().UseLocalTime = x_axis()->type() == Axis::Type::LocalTime;
            }
        }

        ImPlotAxisFlags y_flags = 0;
        if (y_axis()->type() != Axis::Type::Numeric)
        {
            if (y_axis()->type() == Axis::Type::Logarithmic)
                y_flags |= ImPlotAxisFlags_LogScale;
            else
            {
                y_flags |= ImPlotAxisFlags_Time;
                ImPlot::GetStyle().UseLocalTime = y_axis()->type() == Axis::Type::LocalTime;
            }
        }

        if (_x_axis->limits())
            ImPlot::SetNextPlotLimitsX(_x_axis->limits().value()[0], _x_axis->limits().value()[1], ImGuiCond_Always);
        else
            x_flags |= ImPlotAxisFlags_AutoFit;

        if (_y_axis->limits())
            ImPlot::SetNextPlotLimitsY(_y_axis->limits().value()[0], _y_axis->limits().value()[1], ImGuiCond_Always);
        else
            y_flags |= ImPlotAxisFlags_AutoFit;

        if (_x_axis->ticks())
        {
            if (_x_axis->tick_labels())
            {
                auto references = reference_tick_labels(_x_axis->tick_labels().value());
                auto count = int(std::min(_x_axis->ticks().value().size(), _x_axis->tick_labels().value().size()));
                ImPlot::SetNextPlotTicksX(_x_axis->ticks().value().data(), count, references.data(), false);
            }
            else
                ImPlot::SetNextPlotTicksX(_x_axis->ticks().value().data(), int(_x_axis->ticks().value().size()), 0, false);
        }

        if (_y_axis->ticks())
        {
            if (_y_axis->tick_labels())
            {
                auto references = reference_tick_labels(_y_axis->tick_labels().value());
                auto count = int(std::min(_y_axis->ticks().value().size(), _y_axis->tick_labels().value().size()));
                ImPlot::SetNextPlotTicksY(_y_axis->ticks().value().data(), count, references.data(), false);
            }
            else
            {
                ImPlot::SetNextPlotTicksY(_y_axis->ticks().value().data(), int(_y_axis->ticks().value().size()), 0, false);
            }
        }

        if (ImPlot::BeginPlot(
            imgui_label().c_str(),
            _x_axis->label() ? _x_axis->label().value().c_str() : 0,
            _y_axis->label() ? _y_axis->label().value().c_str() : 0,
            size,
            0,
            x_flags,
            y_flags))
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

    std::shared_ptr<Plot::Axis> const& Plot::x_axis() const
    {
        return _x_axis;
    }

    std::shared_ptr<Plot::Axis> const& Plot::y_axis() const
    {
        return _y_axis;
    }

    void Plot::update_content()
    {
        _automatic_height = _automatic_width = 0.f;
    }

    void Plot::Axis::set_label(Label label)
    {
        _label = std::move(label);
    }

    Plot::Label const& Plot::Axis::label() const
    {
        return _label;
    }

    void Plot::Axis::set_limits(Limits limits)
    {
        _limits = std::move(limits);
    }

    Plot::Axis::Axis()
        : Node("PlotAxis")
    {
    }

    void Plot::Axis::set_type(Type type)
    {
        _type = type;
    }

    Plot::Axis::Type Plot::Axis::type() const
    {
        return _type;
    }

    Plot::Limits const& Plot::Axis::limits() const
    {
        return _limits;
    }

    void Plot::Axis::set_ticks(std::optional<Ticks> ticks)
    {
        _ticks = std::move(ticks);
    }

    std::optional<Plot::Ticks> const& Plot::Axis::ticks() const
    {
        return _ticks;
    }

    std::optional<Plot::Ticks>& Plot::Axis::ticks()
    {
        return _ticks;
    }

    void Plot::Axis::set_tick_labels(std::optional<TickLabels> tick_labels)
    {
        _tick_labels = std::move(tick_labels);
    }

    std::optional<Plot::TickLabels> const& Plot::Axis::tick_labels() const
    {
        return _tick_labels;
    }

}