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
#include <p3/UserInterface.h>
#include <p3/Popup.h>
#include <p3/ChildWindow.h>
#include <p3/Theme.h>
#include <p3/MenuBar.h>


namespace p3::python
{

    void Definition<UserInterface>::parse(py::kwargs const& kwargs, UserInterface& user_interface)
    {
    }

    void Definition<UserInterface>::apply(py::module& module)
    {
        py::class_<FontAtlas>(module, "FontAtlas")
            .def("__len__", &FontAtlas::size)
            .def("__getitem__", &FontAtlas::operator[]);

        py::class_<Font>(module, "Font")
            .def_property_readonly("size", &Font::size)
            .def_property_readonly("scale", &Font::scale);

        py::class_<UserInterface, Node, std::shared_ptr<UserInterface>>(module, "UserInterface")
            .def(py::init<>([](
                std::shared_ptr<MenuBar> menu_bar,
                std::shared_ptr<Node> content,
                py::kwargs kwargs) {
                auto user_interface = std::make_shared<UserInterface>();
                user_interface->set_menu_bar(std::move(menu_bar));
                user_interface->set_content(std::move(content));
                return user_interface;
            }), 
                py::kw_only(), 
                py::arg("menu_bar")=py::none(),
                py::arg("content")=py::none())
            .def_property("content", &UserInterface::content, &UserInterface::set_content)
            .def("add", [](UserInterface& user_interface, std::shared_ptr<Popup> popup) {
                user_interface.add(std::move(popup));
            })
            .def("add", [](UserInterface& user_interface, std::shared_ptr<ChildWindow> child_window) {
                user_interface.add(std::move(child_window));
            })
            .def_property("menu_bar", &UserInterface::menu_bar, &UserInterface::set_menu_bar)
            .def_property("theme", &UserInterface::theme, &UserInterface::set_theme)
            .def("load_font", &UserInterface::load_font)
            .def("merge_font", &UserInterface::merge_font)
            .def_property("default_font", &UserInterface::default_font, &UserInterface::set_default_font)
            .def_property("mouse_cursor_scale", &UserInterface::mouse_cursor_scale, &UserInterface::set_mouse_cursor_scale)
            .def_property("anti_aliased_lines", &UserInterface::anti_aliased_lines, &UserInterface::set_anti_aliased_lines)
            .def_property("anti_aliased_fill", &UserInterface::anti_aliased_fill, &UserInterface::set_anti_aliased_fill)
            .def_property("curve_tessellation_tolerance", &UserInterface::curve_tessellation_tolerance, &UserInterface::set_curve_tessellation_tolerance)
            .def_property("circle_tessellation_maximum_error", &UserInterface::circle_tessellation_maximum_error, &UserInterface::set_circle_tessellation_maximum_error);
    }

}
