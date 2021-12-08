
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
        ArgumentParser<Node>()(kwargs, slider);
        assign(kwargs, "format", slider, &Slider<DataType>::set_format);
        assign(kwargs, "value", slider, &Slider<DataType>::set_value);
        assign(kwargs, "min", slider, &Slider<DataType>::set_min);
        assign(kwargs, "max", slider, &Slider<DataType>::set_max);
        assign(kwargs, "on_change", slider, &Slider<DataType>::set_on_change);
    }

    template<typename ValueType>
    void Definition<Slider<ValueType>>::apply(py::module& module)
    {
        auto slider = py::class_<Slider<ValueType>, Node, std::shared_ptr<Slider<ValueType>>>(module, ("Slider" + DataSuffix<ValueType>).c_str());

        slider.def(py::init<>([](py::kwargs kwargs) {
            auto slider = std::make_shared<Slider<ValueType>>();
            parse(kwargs, *slider);
            return slider;
        }));

        def_property(slider, "on_change", &Slider<ValueType>::on_change, &Slider<ValueType>::set_on_change);
        def_property(slider, "format", &Slider<ValueType>::format, &Slider<ValueType>::set_format);
        def_property(slider, "value", &Slider<ValueType>::value, &Slider<ValueType>::set_value);
        def_property(slider, "min", &Slider<ValueType>::min, &Slider<ValueType>::set_min);
        def_property(slider, "max", &Slider<ValueType>::max, &Slider<ValueType>::set_max);
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
