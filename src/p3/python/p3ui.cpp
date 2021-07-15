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


PYBIND11_MODULE(p3ui, module)
{
    using namespace ::p3;

    module.doc() =
        R"docstring(
        Object Oriented ImGUI (https://github.com/0lru/p3)
    )docstring";

    module.def("test", [](int aaaa, int bbbb, int cccc, py::kwargs) {
        std::cout << aaaa << " " << bbbb << " " << cccc << std::endl;
    }, R"doc(
        one is positional
    
        :param aaaa: the a-parameter.
        :param bbbb: the b-parameter.
        :param cccc: the a-parameter.

        :Keyword Arguments:
            * *extra* (``list``) --
              Extra stuff
            * *supplement* (``dict``) --
              Additional content
    )doc", py::arg("aaaa"), py::kw_only(), py::arg("bbbb")=1, py::arg("cccc")=2);

    python::Definition<Context>::apply(module);
    python::Definition<Node>::apply(module);
    python::Definition<Button>::apply(module);
    python::Definition<CheckBox>::apply(module);
    python::Definition<Flexible>::apply(module);
    python::Definition<Texture>::apply(module);
    python::Definition<Image>::apply(module);
    python::Definition<Color>::apply(module);
    python::Definition<ColorEdit>::apply(module);
    python::Definition<ComboBox>::apply(module);
    python::Definition<Tab>::apply(module);
    python::Definition<Table>::apply(module);
    python::Definition<Text>::apply(module);
    python::Definition<Collapsible>::apply(module);
    python::Definition<InputText>::apply(module);
    python::Definition<InputScalar<std::int8_t>>::apply(module);
    python::Definition<InputScalar<std::uint8_t>>::apply(module);
    python::Definition<InputScalar<std::int16_t>>::apply(module);
    python::Definition<InputScalar<std::uint16_t>>::apply(module);
    python::Definition<InputScalar<std::int32_t>>::apply(module);
    python::Definition<InputScalar<std::uint32_t>>::apply(module);
    python::Definition<InputScalar<std::int64_t>>::apply(module);
    python::Definition<InputScalar<std::uint64_t>>::apply(module);
    python::Definition<InputScalar<float>>::apply(module);
    python::Definition<InputScalar<double>>::apply(module);
    python::Definition<Menu>::apply(module);
    python::Definition<MenuItem>::apply(module);
    python::Definition<MenuBar>::apply(module);
    python::Definition<Plot>::apply(module);
    python::Definition<Popup>::apply(module);
    python::Definition<ProgressBar>::apply(module);
    python::Definition<ChildWindow>::apply(module);
    python::Definition<ScrollArea>::apply(module);
    python::Definition<Slider<std::int8_t>>::apply(module);
    python::Definition<Slider<std::uint8_t>>::apply(module);
    python::Definition<Slider<std::int16_t>>::apply(module);
    python::Definition<Slider<std::uint16_t>>::apply(module);
    python::Definition<Slider<std::int32_t>>::apply(module);
    python::Definition<Slider<std::uint32_t>>::apply(module);
    python::Definition<Slider<std::int64_t>>::apply(module);
    python::Definition<Slider<std::uint64_t>>::apply(module);
    python::Definition<Slider<float>>::apply(module);
    python::Definition<Slider<double>>::apply(module);
    python::Definition<StyleBlock>::apply(module);
    python::Definition<Theme>::apply(module);
    python::Definition<Window>::apply(module);
}
