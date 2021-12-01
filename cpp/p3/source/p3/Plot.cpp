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
#include "convert.h"
#include "Context.h"

#include <iostream>
#include <algorithm>

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

    Plot::Colormap const Plot::Colormap::Deep = Colormap(ImPlotColormap_Deep);
    Plot::Colormap const Plot::Colormap::Dark = Colormap(ImPlotColormap_Dark);
    Plot::Colormap const Plot::Colormap::Pastel = Colormap(ImPlotColormap_Pastel);
    Plot::Colormap const Plot::Colormap::Paired = Colormap(ImPlotColormap_Paired);
    Plot::Colormap const Plot::Colormap::Viridis = Colormap(ImPlotColormap_Viridis);
    Plot::Colormap const Plot::Colormap::Plasma = Colormap(ImPlotColormap_Plasma);
    Plot::Colormap const Plot::Colormap::Hot = Colormap(ImPlotColormap_Hot);
    Plot::Colormap const Plot::Colormap::Cool = Colormap(ImPlotColormap_Cool);
    Plot::Colormap const Plot::Colormap::Pink = Colormap(ImPlotColormap_Pink);
    Plot::Colormap const Plot::Colormap::Jet = Colormap(ImPlotColormap_Jet);
    Plot::Colormap const Plot::Colormap::Twilight = Colormap(ImPlotColormap_Twilight);
    Plot::Colormap const Plot::Colormap::RdBu = Colormap(ImPlotColormap_RdBu);
    Plot::Colormap const Plot::Colormap::BrBG = Colormap(ImPlotColormap_BrBG);
    Plot::Colormap const Plot::Colormap::PiYG = Colormap(ImPlotColormap_PiYG);
    Plot::Colormap const Plot::Colormap::Spectral = Colormap(ImPlotColormap_Spectral);
    Plot::Colormap const Plot::Colormap::Greys = Colormap(ImPlotColormap_Greys);


    Plot::Plot()
        : Node("Plot")
        , _x_axis(std::make_shared<Axis>())
        , _y_axis(std::make_shared<Axis>())
        , _legend(std::make_shared<Legend>())
    {
        // 
        // for styling..
        Node::add(_x_axis);
        Node::add(_y_axis);
        Node::add(_legend);
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

        if (_x_axis->auto_fit())
            x_flags |= ImPlotAxisFlags_AutoFit;
        else if(_x_axis->fixed() && _x_axis->limits())
            ImPlot::SetNextAxisLimits(ImAxis_X1, _x_axis->limits().value()[0], _x_axis->limits().value()[1], ImGuiCond_Always);
        else if(!_x_axis->fixed() && _x_axis->check_behavior() && _x_axis->limits())
            ImPlot::SetNextAxisLimits(ImAxis_X1, _x_axis->limits().value()[0], _x_axis->limits().value()[1], ImGuiCond_Always);

        if (_y_axis->auto_fit())
            y_flags |= ImPlotAxisFlags_AutoFit;
        else if(_y_axis->fixed() && _y_axis->limits())
            ImPlot::SetNextAxisLimits(ImAxis_Y1, _y_axis->limits().value()[0], _y_axis->limits().value()[1], ImGuiCond_Always);
        else if(!_y_axis->fixed() && _y_axis->check_behavior() && _y_axis->limits())
            ImPlot::SetNextAxisLimits(ImAxis_Y1, _y_axis->limits().value()[0], _y_axis->limits().value()[1], ImGuiCond_Always);

        ImPlotFlags plot_flags = ImPlotFlags_None;
        if (!legend()->visible())
            plot_flags |= ImPlotFlags_NoLegend;

        if (ImPlot::BeginPlot(
            imgui_label().c_str(),
            _x_axis->label() ? _x_axis->label().value().c_str() : 0,
            _y_axis->label() ? _y_axis->label().value().c_str() : 0,
            size,
            legend()->visible() ? 0 : ImPlotFlags_NoLegend,
            x_flags,
            y_flags))
        {
            if (_x_axis->ticks())
            {
                if (_x_axis->tick_labels())
                {
                    auto references = reference_tick_labels(_x_axis->tick_labels().value());
                    auto count = int(std::min(_x_axis->ticks().value().size(), _x_axis->tick_labels().value().size()));
                    ImPlot::SetupAxisTicks(ImAxis_X1, _x_axis->ticks().value().data(), count, references.data(), false);
                }
                else
                    ImPlot::SetupAxisTicks(ImAxis_X1, _x_axis->ticks().value().data(), int(_x_axis->ticks().value().size()), 0, false);
            }

            if (_y_axis->ticks())
            {
                if (_y_axis->tick_labels())
                {
                    auto references = reference_tick_labels(_y_axis->tick_labels().value());
                    auto count = int(std::min(_y_axis->ticks().value().size(), _y_axis->tick_labels().value().size()));
                    ImPlot::SetupAxisTicks(ImAxis_Y1, _y_axis->ticks().value().data(), count, references.data(), false);
                }
                else
                {
                    ImPlot::SetupAxisTicks(ImAxis_Y1, _y_axis->ticks().value().data(), int(_y_axis->ticks().value().size()), 0, false);
                }
            }


            if (legend()->visible())
            {
                int legend_flags = legend()->style_computation().direction == Direction::Vertical
                    ? ImPlotLegendFlags_None
                    : ImPlotLegendFlags_Horizontal;
                if (legend()->outside())
                    legend_flags |= ImPlotLegendFlags_Outside;
                ImPlot::SetupLegend(static_cast<ImPlotLocation>(legend()->location()), legend_flags);
            }
            for (auto& item : _items)
            {
                if (item->line_color())
                    ImPlot::SetNextLineStyle(item->native_line_color());
                if (item->fill_color())
                    ImPlot::SetNextFillStyle(item->native_fill_color());
                //                if (item->opacity())
                //                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, item->opacity().value());
                item->apply_style();
                item->render();
                //               if (item->opacity())
                //                   ImGui::PopStyleVar(ImGuiStyleVar_Alpha);
                               //for (auto& annotation : item->annotations())
                               //    annotation->render_item_annotation();
            }
            ImPlot::EndPlot();
        }
    }

    void Plot::Item::apply_style()
    {
        //if (opacity())
        //    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, opacity().value());
        auto& context = Context::current();
        ImPlotMarker style = marker_style()
            ? static_cast<ImPlotMarker>(marker_style().value())
            : ImPlotMarker_None;
        float size = marker_size()
            ? context.to_actual(marker_size().value())
            : -1.f;
        float weight = marker_weight()
            ? context.to_actual(marker_weight().value())
            : -1.f;
        ImPlot::GetStyle().Colormap = colormap().index();
        ImPlot::SetNextMarkerStyle(style, size, native_marker_fill_color(), weight, native_marker_line_color());
    }

    void Plot::Item::set_plot(Plot* plot)
    {
        _plot = plot;
    }

    void Plot::Item::redraw()
    {
        if (_plot)
            _plot->redraw();
    }

    void Plot::add(std::shared_ptr<Item> item)
    {
        item->synchronize_with(*this);
        item->set_plot(this);
        _items.push_back(std::move(item));
    }

    void Plot::remove(std::shared_ptr<Item> item)
    {
        _items.erase(std::remove_if(_items.begin(), _items.end(), [&](auto iterated) {
            if (iterated == item)
            {
                item->release();
                item->set_plot(nullptr);
                return true;
            }
            else
            {
                return false;
            }
        }), _items.end());
    }

    void Plot::clear()
    {
        for (auto& item : _items)
        {
            item->release();
            item->set_plot(nullptr);
        }
        _items.clear();
    }

    void Plot::synchronize_with(Synchronizable& synchronizable)
    {
        Node::synchronize_with(synchronizable);
        for (auto& item : _items)
            item->synchronize_with(*this);
    }

    std::shared_ptr<Plot::Axis> const& Plot::x_axis() const
    {
        return _x_axis;
    }

    std::shared_ptr<Plot::Axis> const& Plot::y_axis() const
    {
        return _y_axis;
    }

    std::shared_ptr<Plot::Legend> const& Plot::legend() const
    {
        return _legend;
    }

    void Plot::update_content()
    {
        _automatic_height = _automatic_width = 0.f;
    }

    void Plot::Axis::set_auto_fit(bool auto_fit)
    {
        _auto_fit = auto_fit;
        _check_behavior = true;
    }

    bool Plot::Axis::auto_fit() const
    {
        return _auto_fit;
    }

    void Plot::Axis::set_fixed(bool fixed)
    {
        _fixed = fixed;
        _check_behavior = true;
    }

    bool Plot::Axis::fixed() const
    {
        return _fixed;
    }

    void Plot::Axis::set_label(Label label)
    {
        _label = std::move(label);
    }

    bool Plot::Axis::check_behavior()
    {
        bool check_behavior = false;
        std::swap(check_behavior, _check_behavior);
        return check_behavior;
    }

    Plot::Label const& Plot::Axis::label() const
    {
        return _label;
    }

    void Plot::Axis::set_limits(Limits limits)
    {
        _limits = std::move(limits);
        _check_behavior = true;
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

    void Plot::Annotation::render()
    {
        ImPlot::Annotation(x(), y(), _fill_color, offset(), clamped(), text().c_str());
    }

    void Plot::Annotation::render_item_annotation()
    {
        bool has_line_color = line_color() && fill_color() && fill_color().value().alpha() == 0.f;
        if (has_line_color)
            std::swap(ImPlot::GetStyle().Colors[ImPlotCol_InlayText], native_line_color());
        ImPlot::Annotation(x(), y(), _fill_color, offset(), clamped(), text().c_str());
        if (has_line_color)
            std::swap(ImPlot::GetStyle().Colors[ImPlotCol_InlayText], native_line_color());
    }

    std::optional<Color> Plot::Item::line_color() const
    {
        return convert(_line_color);
    }

    void Plot::Item::set_line_color(std::optional<Color> line_color)
    {
        _line_color = convert(line_color);
    }

    std::optional<Color> Plot::Item::fill_color() const
    {
        return convert(_fill_color);
    }

    void Plot::Item::set_fill_color(std::optional<Color> fill_color)
    {
        _fill_color = convert(fill_color);
    }

    std::optional<float> const& Plot::Item::opacity() const
    {
        return _opacity;
    }

    void Plot::Item::set_opacity(std::optional<float> opacity)
    {
        _opacity = std::move(opacity);
    }

    std::optional<Color> Plot::Item::marker_line_color() const
    {
        return convert(_marker_line_color);
    }

    void Plot::Item::set_marker_line_color(std::optional<Color> marker_line_color)
    {
        _marker_line_color = convert(marker_line_color);
    }

    std::optional<Color> Plot::Item::marker_fill_color() const
    {
        return convert(_marker_fill_color);
    }

    void Plot::Item::set_marker_fill_color(std::optional<Color> marker_fill_color)
    {
        _marker_fill_color = convert(marker_fill_color);
    }

    std::optional<Length> const& Plot::Item::marker_size() const
    {
        return _marker_size;
    }

    void Plot::Item::set_marker_size(std::optional<Length> marker_size)
    {
        _marker_size = std::move(marker_size);
    }

    std::optional<Length> const& Plot::Item::marker_weight() const
    {
        return _marker_weight;
    }

    void Plot::Item::set_marker_weight(std::optional<Length> marker_weight)
    {
        _marker_weight = std::move(marker_weight);
    }

    std::optional<MarkerStyle> const& Plot::Item::marker_style() const
    {
        return _marker_style;
    }

    void Plot::Item::set_marker_style(std::optional<MarkerStyle> marker_style)
    {
        _marker_style = std::move(marker_style);
    }

    void Plot::Item::add(std::shared_ptr<Annotation> annotation)
    {
        if (!annotation)
            return;
        annotation->synchronize_with(*this);
        _annotations.push_back(std::move(annotation));
    }

    void Plot::Item::remove(std::shared_ptr<Annotation> annotation)
    {
        if (!annotation)
            return;
        annotation->release();
        std::erase(_annotations, annotation);
    }

    std::vector<std::shared_ptr<Plot::Annotation>> const& Plot::Item::annotations() const
    {
        return _annotations;
    }

    void Plot::Item::set_annotations(std::vector<std::shared_ptr<Annotation>> annotations)
    {
        _annotations = std::move(annotations);
    }

    Plot::Legend::Legend()
        : Node("Legend")
    {
    }

    void Plot::Legend::set_outside(bool outside)
    {
        _outside = outside;
    }

    bool Plot::Legend::outside() const
    {
        return _outside;
    }

    void Plot::Legend::set_location(Location location)
    {
        _location = location;
    }

    Location Plot::Legend::location() const
    {
        return _location;
    }

    Plot::Colormap::Colormap(int index)
        : _index(index)
    {
    }

    Plot::Colormap::Colormap(std::string const& name, std::vector<Color> colors, bool interpolated)
    {
        std::vector<ImVec4> native_colors(colors.size());
        std::transform(colors.begin(), colors.end(), native_colors.begin(), [](auto const& color) {
            return convert(color);
        });
        _index = ImPlot::AddColormap(name.c_str(), native_colors.data(), int(native_colors.size()), interpolated);
    }

}