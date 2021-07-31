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
#pragma once
#define NOMINMAX

#include "Node.h"
#include "Color.h"

#include <implot.h>

#include <string>
#include <functional>
#include <optional>
#include <variant>


namespace p3
{

    enum class Location : ImPlotLocation
    {
        North = ImPlotLocation_North,
        NorthEast = ImPlotLocation_NorthEast,
        East = ImPlotLocation_East,
        SouthEast = ImPlotLocation_SouthEast,
        South = ImPlotLocation_South,
        SouthWest = ImPlotLocation_SouthWest,
        West = ImPlotLocation_West,
        NorthWest = ImPlotLocation_NorthWest,
    };

    enum class MarkerStyle : ImPlotMarker
    {
        None = ImPlotMarker_None,
        Circle = ImPlotMarker_Circle,
        Square = ImPlotMarker_Square,
        Diamond = ImPlotMarker_Diamond,
        Up = ImPlotMarker_Up,
        Down = ImPlotMarker_Down,
        Left = ImPlotMarker_Left,
        Right = ImPlotMarker_Right,
        Cross = ImPlotMarker_Cross,
        Plus = ImPlotMarker_Plus,
        Asterisk = ImPlotMarker_Asterisk
    };

    class Plot : public Node
    {
    public:
        class Axis;
        class Legend;
        class Annotation;

        using Ticks = std::vector<double>;
        using TickLabels = std::vector<std::string>;

        class Item
        {
        public:
            virtual ~Item() = default;
            virtual void render() = 0;
            virtual void apply_style();

            std::optional<Color> line_color() const;
            ImVec4& native_line_color() { return _line_color; }
            void set_line_color(std::optional<Color>);

            std::optional<Color> fill_color() const;
            void set_fill_color(std::optional<Color>);
            ImVec4& native_fill_color() { return _fill_color; }

            std::optional<float> const& opacity() const;
            void set_opacity(std::optional<float>);

        protected:
            std::optional<float> _opacity = std::nullopt;
            ImVec4 _line_color = IMPLOT_AUTO_COL;
            ImVec4 _fill_color = IMPLOT_AUTO_COL;
        };

        class AnnotatedItem : public Item
        {
        public:
            void add(std::shared_ptr<Annotation>);
            void remove(std::shared_ptr<Annotation>);

            std::vector<std::shared_ptr<Annotation>> const& annotations() const;
            void set_annotations(std::vector<std::shared_ptr<Annotation>>);

            std::optional<Color> marker_line_color() const;
            ImVec4& native_marker_line_color() { return _marker_line_color; }
            void set_marker_line_color(std::optional<Color>);

            std::optional<Color> marker_fill_color() const;
            void set_marker_fill_color(std::optional<Color>);
            ImVec4& native_marker_fill_color() { return _marker_fill_color; }

            std::optional<Length> const& marker_size() const;
            void set_marker_size(std::optional<Length>);

            std::optional<Length> const& marker_weight() const;
            void set_marker_weight(std::optional<Length>);

            std::optional<MarkerStyle> const& marker_style() const;
            void set_marker_style(std::optional<MarkerStyle>);

        protected:
            void apply_style() override;

        private:
            std::vector<std::shared_ptr<Annotation>> _annotations;
            ImVec4 _marker_line_color = IMPLOT_AUTO_COL;
            ImVec4 _marker_fill_color = IMPLOT_AUTO_COL;
            std::optional<Length> _marker_size = std::nullopt;
            std::optional<Length> _marker_weight = std::nullopt;
            std::optional<MarkerStyle> _marker_style = std::nullopt;
        };

        class Annotation : public Item
        {
        public:
            std::string text;
            double x;
            double y;
            ImVec2 offset{ 0.f, 0.f };
            bool clamped = false;
            void render() override;
            void render_item_annotation();
        };

        template<typename T>
        class BarSeries : public AnnotatedItem
        {
        public:
            std::string name;
            std::vector<T> values;
            std::optional<std::vector<T>> errors;
            double shift = 0.;
            double width = 1.;
            void render() override;
        };

        template<typename T>
        class LineSeries : public AnnotatedItem
        {
        public:
            std::string name;
            std::vector<T> x;
            std::vector<T> y;
            void render() override;
        };

        template<typename T>
        class StemSeries : public AnnotatedItem
        {
        public:
            std::string name;
            std::vector<T> x;
            std::vector<T> y;
            void render() override;
        };

        template<typename T>
        class ScatterSeries : public AnnotatedItem
        {
        public:
            std::string name;
            std::vector<T> x;
            std::vector<T> y;
            void render() override;
        };

        template<typename T>
        class HorizontalLines : public AnnotatedItem
        {
        public:
            std::string name;
            std::vector<T> data;
            void render() override;
        };

        template<typename T>
        class VerticalLines : public AnnotatedItem
        {
        public:
            std::string name;
            std::vector<T> data;
            void render() override;
        };

        Plot();

        using Limits = std::optional<std::array<float, 2>>;
        using Label = std::optional<std::string>;

        std::shared_ptr<Axis> const& x_axis() const;
        std::shared_ptr<Axis> const& y_axis() const;
        std::shared_ptr<Legend> const& legend() const;

        void update_content() override;
        void render_impl(Context&, float width, float height) override;

        void add(std::shared_ptr<Item>);
        void remove(std::shared_ptr<Item>);
        void clear();

    private:
        std::string _title;
        std::shared_ptr<Axis> _x_axis;
        std::shared_ptr<Axis> _y_axis;
        std::shared_ptr<Legend> _legend;
        std::vector<std::shared_ptr<Item>> _items;
    };

    class Plot::Legend : public Node
    {
    public:
        Legend();

        void set_location(Location);
        Location location() const;

    private:
        Location _location = Location::NorthWest;
    };

    class Plot::Axis : public Node
    {
    public:
        enum class Type
        {
            Numeric,
            Logarithmic,
            UniversalTime,
            LocalTime
        };

        Axis();

        void set_type(Type);
        Type type() const;

        void set_label(Label);
        Label const& label() const;

        void set_limits(Limits);
        Limits const& limits() const;

        void set_ticks(std::optional<Ticks>);
        std::optional<Ticks> const& ticks() const;
        std::optional<Ticks>& ticks();

        void set_tick_labels(std::optional<TickLabels>);
        std::optional<TickLabels> const& tick_labels() const;

    private:
        Type _type = Type::Numeric;
        Label _label;
        Limits _limits;
        std::optional<Ticks> _ticks = std::nullopt;
        std::optional<TickLabels> _tick_labels = std::nullopt;
    };

    template<typename T>
    void Plot::BarSeries<T>::render()
    {
        auto sample_count = values.size();
        ImPlot::PlotBars(name.c_str(), values.data(), int(values.size()), width, shift);
        for (auto& annotation : annotations())
            annotation->render_item_annotation();
    }

    template<typename T>
    void Plot::LineSeries<T>::render()
    {
        auto sample_count = std::min(x.size(), y.size());
        ImPlot::PlotLine(name.c_str(), x.data(), y.data(), static_cast<int>(sample_count));
        for (auto& annotation : annotations())
            annotation->render_item_annotation();
    }

    template<typename T>
    void Plot::ScatterSeries<T>::render()
    {
        auto count = int(std::min(x.size(), y.size()));
        ImPlot::PlotScatter(name.c_str(), x.data(), y.data(), count);
        for (auto& annotation : annotations())
            annotation->render_item_annotation();
    }

    template<typename T>
    void Plot::StemSeries<T>::render()
    {
        auto sample_count = std::min(x.size(), y.size());
        ImPlot::PlotStems(name.c_str(), x.data(), y.data(), static_cast<int>(sample_count));
        for (auto& annotation : annotations())
            annotation->render_item_annotation();
    }

    template<typename T>
    void Plot::HorizontalLines<T>::render()
    {
        ImPlot::PlotHLines(name.c_str(), data.data(), static_cast<int>(data.size()));
    }

    template<typename T>
    void Plot::VerticalLines<T>::render()
    {
        ImPlot::PlotVLines(name.c_str(), data.data(), static_cast<int>(data.size()));
    }

}
