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

namespace p3::python::skia { void cleanup(); }


PYBIND11_MODULE(_p3ui, module)
{
    using namespace ::p3;

    module.doc() =
        R"docstring(
        Object Oriented ImGUI (https://github.com/0lru/p3ui)
    )docstring";

    py::module_::import("atexit").attr("register")(py::cpp_function([&]() {
        python::skia::cleanup();
    }));

    python::Definition<Node>::apply(module);
    python::Definition<Button>::apply(module);
    python::Definition<CheckBox>::apply(module);
    python::Definition<Layout>::apply(module);
    python::Definition<Texture>::apply(module);
    python::Definition<ToolTip>::apply(module);
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
    python::Definition<UserInterface>::apply(module);
    python::Definition<Window>::apply(module);

    python::Definition<python::Builder>::apply(module);
    python::Definition<python::Surface>::apply(module);
}
