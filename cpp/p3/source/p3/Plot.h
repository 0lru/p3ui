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

#include <implot.h> // TODO: move to cpp

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

        class Colormap
        {
        public:
            static Colormap const Deep;
            static Colormap const Dark;
            static Colormap const Pastel;
            static Colormap const Paired;
            static Colormap const Viridis;
            static Colormap const Plasma;
            static Colormap const Hot;
            static Colormap const Cool;
            static Colormap const Pink;
            static Colormap const Jet;
            static Colormap const Twilight;
            static Colormap const RdBu;
            static Colormap const BrBG;
            static Colormap const PiYG;
            static Colormap const Spectral;
            static Colormap const Greys;

            Colormap(int index = 0);
            Colormap(std::string const& name, std::vector<Color>, bool interpolated);

            Colormap(Colormap const&) = default;
            Colormap(Colormap&&) = default;

            Colormap& operator=(Colormap const&) = default;
            Colormap& operator=(Colormap&&) = default;

            int index() const { return _index; }

        private:
            int _index;
        };

        class Item : public Synchronizable
        {
        public:
            virtual ~Item() = default;
            virtual void render() = 0;
            virtual void apply_style();

            void set_plot(Plot*);
            void redraw();

            std::optional<Color> line_color() const;
            ImVec4& native_line_color() { return _line_color; }
            void set_line_color(std::optional<Color>);

            std::optional<Color> fill_color() const;
            void set_fill_color(std::optional<Color>);
            ImVec4& native_fill_color() { return _fill_color; }

            std::optional<float> const& opacity() const;
            void set_opacity(std::optional<float>);

            void add(std::shared_ptr<Annotation>);
            void remove(std::shared_ptr<Annotation>);

            std::vector<std::shared_ptr<Annotation>> const& annotations() const;
            void set_annotations(std::vector<std::shared_ptr<Annotation>>);

            std::optional<Color> marker_line_color() const;
            void set_marker_line_color(std::optional<Color>);

            std::optional<Color> marker_fill_color() const;
            void set_marker_fill_color(std::optional<Color>);

            std::optional<Length> const& marker_size() const;
            void set_marker_size(std::optional<Length>);

            std::optional<Length> const& marker_weight() const;
            void set_marker_weight(std::optional<Length>);

            std::optional<MarkerStyle> const& marker_style() const;
            void set_marker_style(std::optional<MarkerStyle>);

            ImVec4& native_marker_line_color() { return _marker_line_color; }
            ImVec4& native_marker_fill_color() { return _marker_fill_color; }

            Colormap colormap() const { return _colormap; }
            void set_colormap(Colormap colormap) { _colormap = std::move(colormap); }

        protected:
            Plot* _plot = nullptr;

            Colormap _colormap;
            std::optional<float> _opacity = std::nullopt;
            ImVec4 _line_color = IMPLOT_AUTO_COL;
            ImVec4 _fill_color = IMPLOT_AUTO_COL;
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
            void set_text(std::string text) { _text = std::move(text); }
            std::string const& text() const { return _text; }

            void set_x(double x) { _x = x; }
            double x() const { return _x; }

            void set_y(double y) { _y = y; }
            double y() const { return _y; }

            void set_offset_x(float value) { _offset.x = value; }
            float offset_x() const { return _offset.x; }

            void set_offset_y(float value) { _offset.y = value; }
            float offset_y() const { return _offset.y; }

            void set_clamped(bool clamped) { _clamped = clamped; }
            bool clamped() const { return _clamped; }

            ImVec2 const& offset() const { return _offset; }

            void render() override;
            void render_item_annotation();

        private:
            std::string _text;
            double _x, _y;
            ImVec2 _offset{ 0.f, 0.f };
            bool _clamped = false;
        };

        class Series : public Item
        {
        public:
            void set_name(std::string name) { _name = std::move(name); Item::redraw(); }
            std::string const& name() const { return _name; }

        private:
            std::string _name;
        };

        template<typename Decorated, typename T>
        class Series1D : public Decorated
        {
        public:
            void set_values(std::vector<T> values) { _values = std::move(values); Decorated::redraw(); }
            std::vector<T> const& values() const { return _values; }

        private:
            std::vector<T> _values;
        };

        template<typename Decorated, typename T>
        class Series2D : public Decorated
        {
        public:
            void set_x(std::vector<T>);
            std::vector<T> const& x() const;

            void set_y(std::vector<T>);
            std::vector<T> const& y() const;

        private:
            std::vector<T> _x;
            std::vector<T> _y;
        };

        template<typename T>
        class BarSeries : public Series1D<Series, T>
        {
        public:
            void set_shift(double shift) { _shift = shift; }
            double shift() const { return _shift; }

            void set_width(double width) { _width = width; }
            double width() const { return _width; }

            void set_direction(Direction direction) { _direction = direction; }
            Direction direction() const { return _direction; }

            void render() override;

        private:
            Direction _direction = Direction::Vertical;
            double _shift = 0.;
            double _width = 1.;
        };

        template<typename T>
        class LineSeries : public Series2D<Series, T>
        {
        public:
            void render() override;
        };

        template<typename T>
        class StemSeries : public Series2D<Series, T>
        {
        public:
            void render() override;
        };

        template<typename T>
        class ScatterSeries : public Series2D<Series, T>
        {
        public:
            void render() override;
        };

        template<typename T>
        class HorizontalLines : public Series1D<Series, T>
        {
        public:
            void render() override;
        };

        template<typename T>
        class VerticalLines : public Series1D<Series, T>
        {
        public:
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

        virtual void synchronize_with(Synchronizable&) override;

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

        void set_outside(bool);
        bool outside() const;

    private:
        Location _location = Location::NorthWest;
        bool _outside = false;
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

        void set_fixed(bool);
        bool fixed() const;

        void set_auto_fit(bool);
        bool auto_fit() const;

        void set_label(Label);
        Label const& label() const;

        void set_limits(Limits);
        Limits const& limits() const;

        void set_ticks(std::optional<Ticks>);
        std::optional<Ticks> const& ticks() const;
        std::optional<Ticks>& ticks();

        void set_tick_labels(std::optional<TickLabels>);
        std::optional<TickLabels> const& tick_labels() const;

        bool check_behavior();

        void set_inverted(bool);
        bool inverted() const;

    private:
        Type _type = Type::Numeric;
        bool _auto_fit = true;
        bool _fixed = false;
        bool _inverted = false;
        Label _label;
        Limits _limits;
        std::optional<Ticks> _ticks = std::nullopt;
        std::optional<TickLabels> _tick_labels = std::nullopt;
        bool _check_behavior = true;
    };



    template<typename T>
    void Plot::BarSeries<T>::render()
    {
        auto sample_count = this->values().size();
        if (direction() == Direction::Horizontal)
            ImPlot::PlotBarsH(this->name().c_str(), this->values().data(), int(this->values().size()), _width, _shift);
        else
            ImPlot::PlotBars(this->name().c_str(), this->values().data(), int(this->values().size()), _width, _shift);
        for (auto& annotation : this->annotations())
            annotation->render_item_annotation();
    }

    template<typename Decorated, typename T>
    inline void Plot::Series2D<Decorated, T>::set_x(std::vector<T> x)
    {
        _x = std::move(x);
        Decorated::redraw();
    }

    template<typename Decorated, typename T>
    inline std::vector<T> const& Plot::Series2D<Decorated, T>::x() const
    {
        return _x;
    }

    template<typename Decorated, typename T>
    inline void Plot::Series2D<Decorated, T>::set_y(std::vector<T> y)
    {
        _y = std::move(y);
        Decorated::redraw();
    }

    template<typename Decorated, typename T>
    inline std::vector<T> const& Plot::Series2D<Decorated, T>::y() const
    {
        return _y;
    }

    template<typename T>
    void Plot::LineSeries<T>::render()
    {
        auto sample_count = std::min(this->x().size(), this->y().size());
        ImPlot::PlotLine(this->name().c_str(), this->x().data(), this->y().data(), static_cast<int>(sample_count));
        for (auto& annotation : this->annotations())
            annotation->render_item_annotation();
    }

    template<typename T>
    void Plot::ScatterSeries<T>::render()
    {
        auto count = int(std::min(this->x().size(), this->y().size()));
        ImPlot::PlotScatter(this->name().c_str(), this->x().data(), this->y().data(), count);
        for (auto& annotation : this->annotations())
            annotation->render_item_annotation();
    }

    template<typename T>
    void Plot::StemSeries<T>::render()
    {
        auto sample_count = std::min(this->x().size(), this->y().size());
        ImPlot::PlotStems(this->name().c_str(), this->x().data(), this->y().data(), static_cast<int>(sample_count));
        for (auto& annotation : this->annotations())
            annotation->render_item_annotation();
    }

    template<typename T>
    void Plot::HorizontalLines<T>::render()
    {
        ImPlot::PlotHLines(this->name().c_str(), this->values().data(), static_cast<int>(this->values().size()));
    }

    template<typename T>
    void Plot::VerticalLines<T>::render()
    {
        ImPlot::PlotVLines(this->name().c_str(), this->values().data(), static_cast<int>(this->values().size()));
    }

}
