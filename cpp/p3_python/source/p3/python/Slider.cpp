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
#include <p3/Slider.h>
#include <p3/DataSuffix.h>


namespace p3::python
{

    template<typename T>
    class Definition<Slider<T>>
    {
    public:
        static void parse(py::kwargs const&, Slider<T>&);
        static void apply(py::module&);
    };

    template<typename DataType>
    void Definition<Slider<DataType>>::parse(py::kwargs const& kwargs, Slider<DataType>& slider)
    {
        Definition<Node>::parse(kwargs, slider);
        if (kwargs.contains("format"))
            slider.set_format(kwargs["format"].cast<std::optional<std::string>>());
        if (kwargs.contains("value"))
            slider.set_value(kwargs["value"].cast<DataType>());
        if (kwargs.contains("min"))
            slider.set_min(kwargs["min"].cast<DataType>());
        if (kwargs.contains("max"))
            slider.set_max(kwargs["max"].cast<DataType>());

        if (kwargs.contains("on_change"))
            slider.set_on_change([f=kwargs["on_change"].cast<py::function>()](DataType value)
            {
                py::gil_scoped_acquire acquire;
                f(value);
            });
    }

    template<typename ValueType>
    void Definition<Slider<ValueType>>::apply(py::module& module)
    {
        py::class_<Slider<ValueType>, Node, std::shared_ptr<Slider<ValueType>>>
            slider(module, ("Slider" + DataSuffix<ValueType>).c_str());
        slider.def(py::init<>([](py::kwargs kwargs) {
            auto slider = std::make_shared<Slider<ValueType>>();
            parse(kwargs, *slider);
            return slider;
        }));
        slider.def_property("on_change", &Slider<ValueType>::on_change, [](Slider<ValueType>& slider, py::function f) {
            slider.set_on_change([f{ std::move(f) }](ValueType value) {
                py::gil_scoped_acquire acquire;
                f(value);
            });
        });
        slider.def_property("format", &Slider<ValueType>::format, &Slider<ValueType>::set_format);
        slider.def_property("value", &Slider<ValueType>::value, &Slider<ValueType>::set_value);
        slider.def_property("min", &Slider<ValueType>::min, &Slider<ValueType>::set_min);
        slider.def_property("max", &Slider<ValueType>::max, &Slider<ValueType>::set_max);
    }

    template Definition<Slider<std::uint8_t>>;
    template Definition<Slider<std::int8_t>>;
    template Definition<Slider<std::uint16_t>>;
    template Definition<Slider<std::int16_t>>;
    template Definition<Slider<std::uint32_t>>;
    template Definition<Slider<std::int32_t>>;
    template Definition<Slider<std::uint64_t>>;
    template Definition<Slider<std::int64_t>>;
    template Definition<Slider<float>>;
    template Definition<Slider<double>>;

}