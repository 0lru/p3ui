
#include "p3ui.h"
#include <p3/InputScalar.h>
#include <p3/DataSuffix.h>


namespace p3::python
{

    template<typename T>
    class Definition<InputScalar<T>>
    {
    public:
        static void parse(py::kwargs const&, InputScalar<T>&);
        static void apply(py::module&);
    };

    template<typename T>
    void Definition<InputScalar<T>>::parse(py::kwargs const& kwargs, InputScalar<T>& input_scalar)
    {
        ArgumentParser<Node>()(kwargs, input_scalar);
        assign(kwargs, "format", input_scalar, &InputScalar<T>::set_format);
        assign(kwargs, "value", input_scalar, &InputScalar<T>::set_value);
        assign(kwargs, "step", input_scalar, &InputScalar<T>::set_step);
        assign(kwargs, "on_change", input_scalar, &InputScalar<T>::set_on_change);
    }

    template<typename T>
    void Definition<InputScalar<T>>::apply(py::module& module)
    {
        auto input_scalar = py::class_<InputScalar<T>, Node, std::shared_ptr<InputScalar<T>>>(module, ("Input" + DataSuffix<T>).c_str());

        input_scalar.def(py::init<>([](py::kwargs kwargs) {
            auto input_scalar = std::make_shared<InputScalar<T>>();
            parse(kwargs, *input_scalar);
            return input_scalar;
        }));

        def_property(input_scalar, "on_change", &InputScalar<T>::on_change, &InputScalar<T>::set_on_change);
        def_property(input_scalar, "format", &InputScalar<T>::format, &InputScalar<T>::set_format);
        def_property(input_scalar, "value", &InputScalar<T>::value, &InputScalar<T>::set_value);
        def_property(input_scalar, "step", &InputScalar<T>::step, &InputScalar<T>::set_step);
    }

    template Definition<InputScalar<std::int8_t>>;
    template Definition<InputScalar<std::uint8_t>>;
    template Definition<InputScalar<std::int16_t>>;
    template Definition<InputScalar<std::uint16_t>>;
    template Definition<InputScalar<std::int32_t>>;
    template Definition<InputScalar<std::uint32_t>>;
    template Definition<InputScalar<std::int64_t>>;
    template Definition<InputScalar<std::uint64_t>>;
    template Definition<InputScalar<float>>;
    template Definition<InputScalar<double>>;

}