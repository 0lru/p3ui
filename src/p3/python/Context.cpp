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

#include <p3/Context.h>
#include <p3/Font.h>


namespace p3::python
{

    void Definition<Context>::apply(py::module& module)
    {
        py::class_<Context::FontAtlas>(module, "FontAtlas")
            .def("__len__", &Context::FontAtlas::size)
            .def("__getitem__", &Context::FontAtlas::operator[]);

        py::class_<Context::Font>(module, "Font")
            .def_property_readonly("size", &Context::Font::size)
            .def_property_readonly("scale", &Context::Font::scale);

        py::class_<Context, std::shared_ptr<Context>> (module, "Context")
            .def(py::init<>([]() {
                return std::make_shared<Context>();
            }))
            .def("load_font", &Context::load_font)
            .def("merge_font", &Context::merge_font)
            .def_property("default_font", &Context::default_font, &Context::set_default_font)
            .def_property("mouse_cursor_scale", &Context::mouse_cursor_scale, &Context::set_mouse_cursor_scale)
            .def_property("anti_aliased_lines", &Context::anti_aliased_lines, &Context::set_anti_aliased_lines)
            .def_property("anti_aliased_fill", &Context::anti_aliased_fill, &Context::set_anti_aliased_fill)
            .def_property("curve_tessellation_tolerance", &Context::curve_tessellation_tolerance, &Context::set_curve_tessellation_tolerance)
            .def_property("circle_tessellation_maximum_error", &Context::circle_tessellation_maximum_error, &Context::set_circle_tessellation_maximum_error);
    }

}
