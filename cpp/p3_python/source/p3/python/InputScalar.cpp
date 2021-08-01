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