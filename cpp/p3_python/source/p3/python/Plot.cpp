#include "numpy.h"
#include "p3ui.h"

#include <p3/DataSuffix.h>
#include <p3/widgets/Plot.h>

PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<p3::Plot::Annotation>>);

namespace p3::python {

//
// wrap data plot getters/setters

namespace {

    template <typename Object, typename T>
    auto wrap(void (Object::*member)(std::vector<T>))
    {
        return [member](Object& object, py::array_t<T> const& data) {
            std::vector<T> x;
            copy(data, x);
            return (object.*member)(std::move(x));
        };
    }

    template <typename Object, typename T>
    auto wrap(void (Object::*member)(std::optional<std::vector<T>>))
    {
        return [member](Object& object, py::array_t<T> const& data) {
            if (data.is(py::none()))
                return (object.*member)(std::nullopt);
            std::optional<std::vector<T>> x = std::vector<T>();
            copy(data, x.value());
            return (object.*member)(std::move(x));
        };
    }

    template <typename Object, typename T>
    auto wrap(std::vector<T> const& (Object::*member)() const)
    {
        return [member](Object& object) -> py::array_t<T> {
            auto const& v = (object.*member)();
            return py::array_t<T>(v.size(), v.data()).attr("copy")();
        };
    }

    template <typename Object, typename T>
    auto wrap(std::optional<std::vector<T>> const& (Object::*member)() const)
    {
        return [member](Object& object) -> py::array_t<T> {
            auto const& v = (object.*member)();
            return v ? py::array_t<T>(v.value().size(), v.value().data()).attr("copy")() : py::none();
        };
    }
}

//
// arguments

namespace {
    void parse_axis_kwargs(py::kwargs const& kwargs, Plot::Axis& axis, std::string prefix)
    {
        assign(kwargs, (prefix + "_label").c_str(), axis, &Plot::Axis::set_label);
        assign(kwargs, (prefix + "_type").c_str(), axis, &Plot::Axis::set_type);
        assign(kwargs, (prefix + "_limits").c_str(), axis, &Plot::Axis::set_limits);
        assign(kwargs, (prefix + "_tick_labels").c_str(), axis, &Plot::Axis::set_tick_labels);
        assign(kwargs, (prefix + "_ticks").c_str(), axis, &Plot::Axis::set_ticks);
        assign(kwargs, (prefix + "_ticks_visible").c_str(), axis, &Plot::Axis::set_ticks_visible);
        assign(kwargs, (prefix + "_inverted").c_str(), axis, &Plot::Axis::set_inverted);
        assign(kwargs, (prefix + "_fixed").c_str(), axis, &Plot::Axis::set_fixed);
        assign(kwargs, (prefix + "_opposite").c_str(), axis, &Plot::Axis::set_opposite);
        assign(kwargs, (prefix + "_auto_fit").c_str(), axis, &Plot::Axis::set_auto_fit);
        if (kwargs.contains(prefix + "_ticks")) {
            auto x_ticks = kwargs[(prefix + "_ticks").c_str()].cast<std::optional<py::array_t<double>>>();
            copy(x_ticks, axis.ticks());
        }
    }
}

void ArgumentParser<Plot>::operator()(py::kwargs const& kwargs, Plot& plot)
{
    ArgumentParser<Node>()(kwargs, plot);
    parse_axis_kwargs(kwargs, *plot.x_axis(), "x");
    parse_axis_kwargs(kwargs, *plot.y_axis(), "y");
    assign(kwargs, "legend_visible", static_cast<Node&>(*plot.legend()), &Node::set_visible);
    assign(kwargs, "legend_location", *plot.legend(), &Plot::Legend::set_location);
    assign(kwargs, "legend_outside", *plot.legend(), &Plot::Legend::set_outside);
    assign(kwargs, "legend_direction", *plot.legend()->style(), &StyleBlock::set_direction);
}

void ArgumentParser<Plot::Item>::operator()(py::kwargs const& kwargs, Plot::Item& item)
{
    assign(kwargs, "opacity", item, &Plot::Item::set_opacity);
    assign(kwargs, "line_color", item, &Plot::Item::set_line_color);
    assign(kwargs, "line_weight", item, &Plot::Item::set_line_weight);
    assign(kwargs, "fill_color", item, &Plot::Item::set_fill_color);
    assign(kwargs, "marker_line_color", item, &Plot::Item::set_marker_line_color);
    assign(kwargs, "marker_fill_color", item, &Plot::Item::set_marker_fill_color);
    assign(kwargs, "marker_weight", item, &Plot::Item::set_marker_weight);
    assign(kwargs, "marker_size", item, &Plot::Item::set_marker_size);
    assign(kwargs, "marker_style", item, &Plot::Item::set_marker_style);
    assign(kwargs, "colormap", item, &Plot::Item::set_colormap);
}

template <typename T>
struct DefineBarSeries {
    template <typename Module>
    void operator()(Module& module)
    {
        auto class_name = "BarSeries" + DataSuffix<T>;
        auto series = py::class_<Plot::BarSeries<T>, Plot::Item, std::shared_ptr<Plot::BarSeries<T>>>(module, class_name.c_str());
        series.def(py::init<>([](std::string name, py::kwargs kwargs) {
            auto series = std::make_shared<Plot::BarSeries<T>>();
            series->set_name(std::move(name));
            parse_kwargs<Plot::Item>(kwargs, *series);
            assign(kwargs, "values", static_cast<Plot::Series1D<Plot::Series, T>&>(*series), &Plot::Series1D<Plot::Series, T>::set_values);
            assign(kwargs, "shift", *series, &Plot::BarSeries<T>::set_shift);
            assign(kwargs, "width", *series, &Plot::BarSeries<T>::set_width);
            assign(kwargs, "direction", *series, &Plot::BarSeries<T>::set_direction);
            return series;
        }));
        def_property(series, "shift", &Plot::BarSeries<T>::shift, &Plot::BarSeries<T>::set_shift);
        def_property(series, "width", &Plot::BarSeries<T>::width, &Plot::BarSeries<T>::set_width);
        series.def_property("values", wrap<Plot::BarSeries<T>>(&Plot::BarSeries<T>::values), wrap<Plot::BarSeries<T>>(&Plot::BarSeries<T>::set_values));
    }
};

template <const char* prefix, typename Type, typename T>
class DefineSeries1D {
public:
    template <typename Module>
    void operator()(Module& module)
    {
        auto class_name = prefix + DataSuffix<T>;
        auto lines = py::class_<Type, Plot::Series, std::shared_ptr<Type>>(module, class_name.c_str());
        lines.def(py::init<>([](std::string name, py::kwargs kwargs) {
            auto series = std::make_shared<Type>();
            series->set_name(std::move(name));
            ArgumentParser<Plot::Item>()(kwargs, *series);
            assign(kwargs, "values", static_cast<Plot::Series1D<Plot::Series, T>&>(*series), &Plot::Series1D<Plot::Series, T>::set_values);
            return series;
        }));
        lines.def_property("values", wrap<Type>(&Type::values), wrap<Type>(&Type::set_values));
    }
};

template <const char* prefix, typename Type, typename T>
class DefineSeries2D {
public:
    template <typename Module>
    void operator()(Module& module)
    {
        auto class_name = prefix + DataSuffix<T>;
        auto series = py::class_<Type, Plot::Series, std::shared_ptr<Type>>(module, class_name.c_str());

        series.def(py::init<>([](std::string name, py::kwargs kwargs) {
            auto series = std::make_shared<Type>();
            series->set_name(std::move(name));
            assign(kwargs, "x", static_cast<Plot::Series2D<Plot::Series, T>&>(*series), &Plot::Series2D<Plot::Series, T>::set_x);
            assign(kwargs, "y", static_cast<Plot::Series2D<Plot::Series, T>&>(*series), &Plot::Series2D<Plot::Series, T>::set_y);
            parse_kwargs<Plot::Item>(kwargs, *series);
            return series;
        }));
        series.def_property("x", wrap<Type>(&Type::x), wrap<Type>(&Type::set_x));
        series.def_property("y", wrap<Type>(&Type::y), wrap<Type>(&Type::set_y));
    }
};

const char StemSeriesPrefix[] = "StemSeries";
const char LineSeriesPrefix[] = "LineSeries";
const char ScatterSeriesPrefix[] = "ScatterSeries";
const char HorizontalLinesPrefix[] = "HorizontalLines";
const char VerticalLinesPrefix[] = "VerticalLines";

template <typename T>
struct DefineStemSeries : public DefineSeries2D<StemSeriesPrefix, Plot::StemSeries<T>, T> {
};
template <typename T>
struct DefineLineSeries : public DefineSeries2D<LineSeriesPrefix, Plot::LineSeries<T>, T> {
};
template <typename T>
struct DefineScatterSeries : public DefineSeries2D<ScatterSeriesPrefix, Plot::ScatterSeries<T>, T> {
};
template <typename T>
struct DefineHorizontalLines : public DefineSeries1D<HorizontalLinesPrefix, Plot::HorizontalLines<T>, T> {
};
template <typename T>
struct DefineVerticalLines : public DefineSeries1D<VerticalLinesPrefix, Plot::VerticalLines<T>, T> {
};

void Definition<Plot>::apply(py::module& module)
{
    py::enum_<Location>(module, "Location")
        .value("North", Location::North)
        .value("NorthEast", Location::NorthEast)
        .value("East", Location::East)
        .value("SouthEast", Location::SouthEast)
        .value("South", Location::South)
        .value("SouthWest", Location::SouthWest)
        .value("West", Location::West)
        .value("NorthWest", Location::NorthWest)
        .export_values();

    py::enum_<MarkerStyle>(module, "MarkerStyle")
        .value("None", MarkerStyle::None)
        .value("Circle", MarkerStyle::Circle)
        .value("Square", MarkerStyle::Square)
        .value("Diamond", MarkerStyle::Diamond)
        .value("Up", MarkerStyle::Up)
        .value("Down", MarkerStyle::Down)
        .value("Left", MarkerStyle::Left)
        .value("Right", MarkerStyle::Right)
        .value("Cross", MarkerStyle::Cross)
        .value("Plus", MarkerStyle::Plus)
        .value("Asterisk", MarkerStyle::Asterisk)
        .export_values();

    py::class_<Plot, Node, std::shared_ptr<Plot>> plot(module, "Plot");
    plot.def(py::init<>([](py::kwargs kwargs) {
        auto plot = std::make_shared<Plot>();
        ArgumentParser<Plot>()(kwargs, *plot);
        return plot;
    }));
    def_method(plot, "add", &Plot::add);
    def_method(plot, "remove", &Plot::remove);
    def_method(plot, "clear", &Plot::clear);
    plot.def_property_readonly("x_axis", &Plot::x_axis);
    plot.def_property_readonly("y_axis", &Plot::y_axis);
    plot.def_property_readonly("legend", &Plot::legend);

    py::class_<Plot::Axis, Node, std::shared_ptr<Plot::Axis>> axis(plot, "Axis");
    axis.def_property("ticks", wrap(&Plot::Axis::ticks), wrap(&Plot::Axis::set_ticks));
    def_property(axis, "type", &Plot::Axis::type, &Plot::Axis::set_type);
    def_property(axis, "tick_labels", &Plot::Axis::tick_labels, &Plot::Axis::set_tick_labels);
    def_property(axis, "ticks_visible", &Plot::Axis::ticks_visible, &Plot::Axis::set_ticks_visible);
    def_property(axis, "label", &Plot::Axis::label, &Plot::Axis::set_label);
    def_property(axis, "limits", &Plot::Axis::limits, &Plot::Axis::set_limits);
    def_property(axis, "fixed", &Plot::Axis::fixed, &Plot::Axis::set_fixed);
    def_property(axis, "opposite", &Plot::Axis::opposite, &Plot::Axis::set_opposite);
    def_property(axis, "inverted", &Plot::Axis::inverted, &Plot::Axis::set_inverted);
    def_property(axis, "auto_fit", &Plot::Axis::auto_fit, &Plot::Axis::set_auto_fit);

    py::enum_<Plot::Axis::Type>(axis, "Type")
        .value("Numeric", Plot::Axis::Type::Numeric)
        .value("Logarithmic", Plot::Axis::Type::Logarithmic)
        .value("LocalTime", Plot::Axis::Type::LocalTime)
        .value("UniversalTime", Plot::Axis::Type::UniversalTime)
        .export_values();

    auto colormap = py::class_<Plot::Colormap>(plot, "Colormap");
    colormap.def(py::init<>());
    colormap.def(py::init<std::string const&, std::vector<Color>, bool>());
    colormap.def_readonly_static("Deep", &Plot::Colormap::Deep);
    colormap.def_readonly_static("Dark", &Plot::Colormap::Dark);
    colormap.def_readonly_static("Pastel", &Plot::Colormap::Pastel);
    colormap.def_readonly_static("Paired", &Plot::Colormap::Paired);
    colormap.def_readonly_static("Viridis", &Plot::Colormap::Viridis);
    colormap.def_readonly_static("Plasma", &Plot::Colormap::Plasma);
    colormap.def_readonly_static("Hot", &Plot::Colormap::Hot);
    colormap.def_readonly_static("Cool", &Plot::Colormap::Cool);
    colormap.def_readonly_static("Pink", &Plot::Colormap::Pink);
    colormap.def_readonly_static("Jet", &Plot::Colormap::Jet);
    colormap.def_readonly_static("Twilight", &Plot::Colormap::Twilight);
    colormap.def_readonly_static("RdBu", &Plot::Colormap::RdBu);
    colormap.def_readonly_static("BrBG", &Plot::Colormap::BrBG);
    colormap.def_readonly_static("PiYG", &Plot::Colormap::PiYG);
    colormap.def_readonly_static("Spectral", &Plot::Colormap::Spectral);
    colormap.def_readonly_static("Greys", &Plot::Colormap::Greys);

    auto legend = py::class_<Plot::Legend, Node, std::shared_ptr<Plot::Legend>>(plot, "Legend");
    def_property(legend, "location", &Plot::Legend::location, &Plot::Legend::set_location);
    def_property(legend, "outside", &Plot::Legend::outside, &Plot::Legend::set_outside);

    auto plot_item = py::class_<Plot::Item, std::shared_ptr<Plot::Item>>(plot, "Item");
    def_method(plot_item, "add", &Plot::Item::add);
    def_method(plot_item, "remove", &Plot::Item::remove);
    def_property(plot_item, "opacity", &Plot::Item::opacity, &Plot::Item::set_opacity);
    def_property(plot_item, "line_color", &Plot::Item::line_color, &Plot::Item::set_line_color);
    def_property(plot_item, "line_weight", &Plot::Item::line_weight, &Plot::Item::set_line_weight);
    def_property(plot_item, "fill_color", &Plot::Item::fill_color, &Plot::Item::set_fill_color);
    def_property(plot_item, "annotations", &Plot::Item::annotations, &Plot::Item::set_annotations);
    def_property(plot_item, "marker_style", &Plot::Item::marker_style, &Plot::Item::set_marker_style);
    def_property(plot_item, "marker_line_color", &Plot::Item::marker_line_color, &Plot::Item::set_marker_line_color);
    def_property(plot_item, "marker_fill_color", &Plot::Item::marker_fill_color, &Plot::Item::set_marker_line_color);
    def_property(plot_item, "marker_size", &Plot::Item::marker_size, &Plot::Item::set_marker_size);
    def_property(plot_item, "marker_weight", &Plot::Item::marker_weight, &Plot::Item::set_marker_weight);
    def_property(plot_item, "colormap", &Plot::Item::colormap, &Plot::Item::set_colormap);

    auto plot_series = py::class_<Plot::Series, Plot::Item, std::shared_ptr<Plot::Series>>(plot, "Series");
    def_property(plot_series, "name", &Plot::Series::name, &Plot::Series::set_name);

    auto annotation = py::class_<Plot::Annotation, Plot::Item, std::shared_ptr<Plot::Annotation>>(plot, "Annotation");
    annotation.def(py::init<>([](std::string text, py::kwargs kwargs) {
        auto annotation = std::make_shared<Plot::Annotation>();
        annotation->set_text(std::move(text));
        ArgumentParser<Plot::Item>()(kwargs, *annotation);
        assign(kwargs, "x", *annotation, &Plot::Annotation::set_x);
        assign(kwargs, "y", *annotation, &Plot::Annotation::set_y);
        assign(kwargs, "offset_x", *annotation, &Plot::Annotation::set_offset_x);
        assign(kwargs, "offset_y", *annotation, &Plot::Annotation::set_offset_y);
        assign(kwargs, "clamped", *annotation, &Plot::Annotation::set_clamped);
        return annotation;
    }),
        py::arg("text") = std::string());
    def_property(annotation, "text", &Plot::Annotation::text, &Plot::Annotation::set_text);
    def_property(annotation, "x", &Plot::Annotation::x, &Plot::Annotation::set_x);
    def_property(annotation, "y", &Plot::Annotation::y, &Plot::Annotation::set_y);
    def_property(annotation, "offset_x", &Plot::Annotation::offset_x, &Plot::Annotation::set_offset_x);
    def_property(annotation, "offset_y", &Plot::Annotation::offset_y, &Plot::Annotation::set_offset_y);
    def_property(annotation, "clamped", &Plot::Annotation::clamped, &Plot::Annotation::set_clamped);

    p3::invoke_for_all_data_types<DefineStemSeries>(plot);
    p3::invoke_for_all_data_types<DefineLineSeries>(plot);
    p3::invoke_for_all_data_types<DefineScatterSeries>(plot);
    p3::invoke_for_all_data_types<DefineBarSeries>(plot);
    p3::invoke_for_all_data_types<DefineHorizontalLines>(plot);
    p3::invoke_for_all_data_types<DefineVerticalLines>(plot);

    py::bind_vector<std::vector<std::shared_ptr<Plot::Annotation>>>(plot, "AnnotationList");
}

}
