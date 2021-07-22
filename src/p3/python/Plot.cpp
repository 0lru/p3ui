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

#include "p3ui.h"
#include <p3/Plot.h>
#include <p3/DataSuffix.h>


namespace p3::python
{

    namespace
    {

        template<typename T>
        py::capsule make_capsule(std::shared_ptr<T> ptr)
        {
            return py::capsule(new std::shared_ptr<T>(ptr), [](void* ptr) {
                delete reinterpret_cast<std::shared_ptr<T>*>(ptr);
            });
        }

        template<typename Guard, typename T>
        py::array_t<T> make_guarded_array_overlay(std::shared_ptr<Guard> guard, std::vector<T>& vector)
        {
            return py::array_t<T>({ vector.size() }, { sizeof(T) }, vector.data(), make_capsule(guard));
        }

        template<typename T>
        void copy(py::array_t<T> array, std::vector<T>& vector)
        {
            auto in = array.request();
            if (in.shape.size() != 1)
                throw std::invalid_argument("array has wrong shape");
            auto src = reinterpret_cast<T*>(in.ptr);
            vector.resize(in.shape[0]);
            std::copy(src, &src[in.shape[0]], vector.data());
        }

        template<typename T>
        void copy(std::optional<py::array_t<T>> array, std::optional<std::vector<T>>& vector)
        {
            if (!array)
            {
                vector.reset();
            }
            else
            {
                if (!vector)
                    vector = std::vector<T>();
                copy(array.value(), vector.value());
            }
        }

    }

    void Definition<Plot>::parse(py::kwargs const& kwargs, Plot& plot)
    {
        Definition<Node>::parse(kwargs, plot);
        if (kwargs.contains("x_label"))
            plot.x_axis()->set_label(kwargs["x_label"].cast<Plot::Label>());
        if (kwargs.contains("y_label"))
            plot.y_axis()->set_label(kwargs["y_label"].cast<Plot::Label>());
        if (kwargs.contains("x_limits"))
            plot.x_axis()->set_limits(kwargs["x_limits"].cast<Plot::Limits>());
        if (kwargs.contains("y_limits"))
            plot.y_axis()->set_limits(kwargs["y_limits"].cast<Plot::Limits>());
        if (kwargs.contains("x_tick_labels"))
            plot.x_axis()->set_tick_labels(kwargs["x_tick_labels"].cast<std::optional<Plot::TickLabels>>());
        if (kwargs.contains("y_tick_labels"))
            plot.y_axis()->set_tick_labels(kwargs["y_tick_labels"].cast<std::optional<Plot::TickLabels>>());
        if (kwargs.contains("x_ticks"))
        {
            auto x_ticks = kwargs["x_ticks"].cast<std::optional<py::array_t<double>>>();
            copy(x_ticks, plot.x_axis()->ticks());
        }
        if (kwargs.contains("y_ticks"))
        {
            auto y_ticks = kwargs["y_ticks"].cast<std::optional<py::array_t<double>>>();
            copy(y_ticks, plot.y_axis()->ticks());
        }
    }

    template<typename T>
    struct define_bar_series
    {
        template<typename Class>
        void operator()(Class& class_) 
        {
            auto class_name = "BarSeries" + DataSuffix<T>;
            py::class_<Plot::BarSeries<T>, 
                Plot::Item, 
                std::shared_ptr<Plot::BarSeries<T>>>(class_, class_name.c_str())
                .def(py::init<>([](std::string name, py::kwargs kwargs) {
                    auto series = std::make_shared<Plot::BarSeries<T>>();
                    series->name = std::move(name);
                    if(kwargs.contains("values"))
                        copy(kwargs["values"].cast<py::array_t<T>>(), series->values);
                    if (kwargs.contains("shift"))
                        series->shift = kwargs["shift"].cast<double>();
                    if (kwargs.contains("width"))
                        series->width = kwargs["width"].cast<double>();
                    return series;
                }))
                .def_readwrite("name", &Plot::BarSeries<T>::name)
                .def_readwrite("shift", &Plot::BarSeries<T>::shift)
                .def_readwrite("width", &Plot::BarSeries<T>::width)
                .def_property("values", [](std::shared_ptr<Plot::BarSeries<T>> series) {
                    return make_guarded_array_overlay(series, series->values);
                }, [](Plot::BarSeries<T>& series, py::array_t<T> array) {
                    copy(array, series.values);
                });
        }
    };

    template<typename T>
    struct define_line_series
    {
        template<typename Class>
        void operator()(Class& class_) 
        {
            auto class_name = "LineSeries" + DataSuffix<T>;
            py::class_<
                Plot::LineSeries<T>, 
                Plot::Item, 
                std::shared_ptr<Plot::LineSeries<T>>>(class_, class_name.c_str())
                .def(py::init<>([](std::string name, py::kwargs kwargs) {
                    auto series = std::make_shared<Plot::LineSeries<T>>();
                    series->name = std::move(name);
                    if(kwargs.contains("x"))
                        copy(kwargs["x"].cast<py::array_t<T>>(), series->x);
                    if(kwargs.contains("y"))
                        copy(kwargs["y"].cast<py::array_t<T>>(), series->y);
                    return series;
                }))
                .def_readwrite("name", &Plot::LineSeries<T>::name)
                .def_property("x", [](std::shared_ptr<Plot::LineSeries<T>> series) {
                    return make_guarded_array_overlay(series, series->x);
                }, [](Plot::LineSeries<T>& series, py::array_t<T> array) {
                    copy(array, series.x);
                })
                .def_property("y", [](std::shared_ptr<Plot::LineSeries<T>> series) {
                    return make_guarded_array_overlay(series, series->y);
                }, [](Plot::LineSeries<T>& series, py::array_t<T> array) {
                    copy(array, series.y);
                });
        }
    };

    template<typename T>
    struct define_scatter_series
    {
        template<typename Class>
        void operator()(Class& class_) 
        {
            auto class_name = "ScatterSeries" + DataSuffix<T>;
        py::class_<Plot::ScatterSeries<T>, 
            Plot::Item,
            std::shared_ptr<Plot::ScatterSeries<T>>>(class_, class_name.c_str())
            .def(py::init<>([](std::string name, py::kwargs kwargs) {
                auto series = std::make_shared<Plot::ScatterSeries<T>>();
                series->name = std::move(name);
                if(kwargs.contains("x"))
                    copy(kwargs["x"].cast<py::array_t<T>>(), series->x);
                if(kwargs.contains("y"))
                    copy(kwargs["y"].cast<py::array_t<T>>(), series->y);
                return series;
            }))
            .def_readwrite("name", &Plot::ScatterSeries<T>::name)
            .def_readwrite("marker_style", &Plot::ScatterSeries<T>::marker_style)
            .def_property("x", [](std::shared_ptr<Plot::ScatterSeries<T>> series) {
                return make_guarded_array_overlay(series, series->x);
            }, [](Plot::ScatterSeries<T>& series, py::array_t<T> array) {
                copy(array, series.x);
            })
            .def_property("y", [](std::shared_ptr<Plot::ScatterSeries<T>> series) {
                return make_guarded_array_overlay(series, series->y);
            }, [](Plot::ScatterSeries<T>& series, py::array_t<T> array) {
                copy(array, series.y);
            });
        }
    };

    template<typename T>
    struct define_stem_series
    {
        template<typename Class>
        void operator()(Class& class_) 
        {
            auto class_name = "StemSeries" + DataSuffix<T>;
            py::class_<Plot::StemSeries<T>, 
                Plot::Item, 
                std::shared_ptr<Plot::StemSeries<T>>>(class_, class_name.c_str())
                .def(py::init<>([](std::string name, py::kwargs kwargs) {
                    auto series = std::make_shared<Plot::StemSeries<T>>();
                    series->name = std::move(name);
                    if(kwargs.contains("x"))
                        copy(kwargs["x"].cast<py::array_t<T>>(), series->x);
                    if(kwargs.contains("y"))
                        copy(kwargs["y"].cast<py::array_t<T>>(), series->y);
                    return series;
                }))
                .def_readwrite("name", &Plot::StemSeries<T>::name)
                .def_property("x", [](std::shared_ptr<Plot::StemSeries<T>> series) {
                    return make_guarded_array_overlay(series, series->x);
                }, [](Plot::StemSeries<T>& series, py::array_t<T> array) {
                    copy(array, series.x);
                })
                .def_property("y", [](std::shared_ptr<Plot::StemSeries<T>> series) {
                    return make_guarded_array_overlay(series, series->y);
                }, [](Plot::StemSeries<T>& series, py::array_t<T> array) {
                    copy(array, series.y);
                });
        }
    };

    template<typename T>
    struct define_horizontal_lines
    {
        template<typename Class>
        void operator()(Class& class_) 
        {
            auto class_name = "HorizontalLines" + DataSuffix<T>;
            py::class_<Plot::HorizontalLines<T>, 
                Plot::Item, 
                std::shared_ptr<Plot::HorizontalLines<T>>>(class_, class_name.c_str())
                .def(py::init<>([](std::string name, py::kwargs kwargs) {
                    auto series = std::make_shared<Plot::HorizontalLines<T>>();
                    series->name = std::move(name);
                    if(kwargs.contains("data"))
                        copy(kwargs["data"].cast<py::array_t<T>>(), series->data);
                    return series;
                }))
                .def_property("name", [](Plot::HorizontalLines<T>& lines) {
                    return lines.name;
                }, [](Plot::HorizontalLines<T>& lines, std::string name) {
                    lines.name = std::move(name);
                })
                .def_property("data", [](std::shared_ptr<Plot::HorizontalLines<T>> lines) {
                    return make_guarded_array_overlay(lines, lines->data);
                }, [](Plot::HorizontalLines<T>& lines, py::array_t<T> array) {
                    copy(array, lines.data);
                });
        }
    };

    template<typename T>
    struct define_vertical_lines
    {
        template<typename Class>
        void operator()(Class& class_) 
        {
            auto class_name = "VerticalLines" + DataSuffix<T>;
            py::class_<Plot::VerticalLines<T>, 
                Plot::Item, 
                std::shared_ptr<Plot::VerticalLines<T>>>(class_, class_name.c_str())
                .def(py::init<>([](std::string name, py::kwargs kwargs) {
                    auto series = std::make_shared<Plot::VerticalLines<T>>();
                    series->name = std::move(name);
                    if(kwargs.contains("data"))
                        copy(kwargs["data"].cast<py::array_t<T>>(), series->data);
                    return series;
                }))
                .def_property("name", [](Plot::VerticalLines<T>& lines) {
                    return lines.name;
                }, [](Plot::VerticalLines<T>& lines, std::string name) {
                    lines.name = std::move(name);
                })
                .def_property("data", [](std::shared_ptr<Plot::VerticalLines<T>> lines) {
                    return make_guarded_array_overlay(lines, lines->data);
                }, [](Plot::VerticalLines<T>& lines, py::array_t<T> array) {
                    copy(array, lines.data);
                });
        }
    };

    void Definition<Plot>::apply(py::module& module)
    {
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
        plot
            .def(py::init<>([](py::kwargs kwargs) {
                auto plot = std::make_shared<Plot>();
                parse(kwargs, *plot);
                return plot;
            }))
            .def("add", &Plot::add)
            .def("remove", &Plot::remove)
            .def("clear", &Plot::clear);

        py::class_< Plot::Axis, Node, std::shared_ptr<Plot::Axis>>(plot, "Axis")
            .def_property("ticks", [](std::shared_ptr<Plot::Axis> axis) {
                using ResultType = std::optional<py::array_t<double>>;
                return axis->ticks()
                    ? ResultType(make_guarded_array_overlay(axis, axis->ticks().value()))
                    : ResultType();
            }, [](std::shared_ptr<Plot::Axis> axis, std::optional<py::array_t<double>> array) {
                copy(array, axis->ticks());
            })
            .def_property("tick_labels", &Plot::Axis::tick_labels, &Plot::Axis::set_tick_labels)
                .def_property("label", &Plot::Axis::label, &Plot::Axis::set_label)
                .def_property("limits", &Plot::Axis::limits, &Plot::Axis::set_limits);

        auto plot_item = py::class_<Plot::Item, std::shared_ptr<Plot::Item>>(plot, "Item");

        p3::invoke_for_all_data_types<define_bar_series>(plot);
        p3::invoke_for_all_data_types<define_line_series>(plot);
        p3::invoke_for_all_data_types<define_scatter_series>(plot);
        p3::invoke_for_all_data_types<define_stem_series>(plot);

        p3::invoke_for_all_data_types<define_horizontal_lines>(plot);
        p3::invoke_for_all_data_types<define_vertical_lines>(plot);
    }

}