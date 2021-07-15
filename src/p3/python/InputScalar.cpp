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
        Definition<Node>::parse(kwargs, input_scalar);
        if (kwargs.contains("format"))
            input_scalar.set_format(kwargs["format"].cast<std::optional<std::string>>());
        if (kwargs.contains("value"))
            input_scalar.set_value(kwargs["value"].cast<T>());
        if (kwargs.contains("step"))
            input_scalar.set_step(kwargs["step"].cast<std::optional<T>>());
        if (kwargs.contains("on_change"))
        {
            input_scalar.set_on_change([f{kwargs["on_change"].cast<py::function>()}](T value)
            {
                py::gil_scoped_acquire acquire;
                f(value);
            });
        }
    }

    template<typename T>
    void Definition<InputScalar<T>>::apply(py::module& module)
    {
        py::class_<InputScalar<T>, Node, std::shared_ptr<InputScalar<T>>> 
            input_scalar(module, ("Input" + DataSuffix<T>).c_str());
        input_scalar.def(py::init<>([](py::kwargs kwargs) {
            auto input_scalar = std::make_shared<InputScalar<T>>();
            parse(kwargs, *input_scalar);
            return input_scalar;
        }));
        input_scalar.def_property("on_change", &InputScalar<T>::on_change, [](InputScalar<T>& input, py::function f) {
            input.set_on_change([f{ std::move(f) }](T value){
                py::gil_scoped_acquire acquire;
                f(value);
            });
        });
        input_scalar.def_property("format", &InputScalar<T>::format, &InputScalar<T>::set_format);
        input_scalar.def_property("value", &InputScalar<T>::value, &InputScalar<T>::set_value);
        input_scalar.def_property("step", &InputScalar<T>::step, &InputScalar<T>::set_step);
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