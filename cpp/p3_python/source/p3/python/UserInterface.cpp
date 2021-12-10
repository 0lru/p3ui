#include "p3ui.h"

#include <p3/Theme.h>
#include <p3/UserInterface.h>

#include <p3/widgets/Popup.h>
#include <p3/widgets/ChildWindow.h>
#include <p3/widgets/MenuBar.h>


namespace p3::python
{

    void Definition<UserInterface>::apply(py::module& module)
    {
        py::class_<FontAtlas>(module, "FontAtlas")
            .def("__len__", &FontAtlas::size)
            .def("__getitem__", &FontAtlas::operator[]);

        py::class_<Font>(module, "Font")
            .def_property_readonly("size", &Font::size)
            .def_property_readonly("scale", &Font::scale);

        auto user_interface = py::class_<UserInterface, Node, std::shared_ptr<UserInterface>>(module, "UserInterface");

        user_interface.def(py::init<>([](std::shared_ptr<MenuBar> menu_bar, std::shared_ptr<Node> content, py::kwargs kwargs)
        {
            auto user_interface = std::make_shared<UserInterface>();
            user_interface->set_menu_bar(std::move(menu_bar));
            user_interface->set_content(std::move(content));
            return user_interface;
        }), py::kw_only(), py::arg("menu_bar") = py::none(), py::arg("content") = py::none());

        def_property(user_interface, "content", &UserInterface::content, &UserInterface::set_content);
        def_method(user_interface, "load_font", &UserInterface::load_font);
        def_method(user_interface, "merge_font", &UserInterface::merge_font);
        def_property(user_interface, "menu_bar", &UserInterface::menu_bar, &UserInterface::set_menu_bar);
        def_property(user_interface, "theme", &UserInterface::theme, &UserInterface::set_theme);
        def_property(user_interface, "default_font", &UserInterface::default_font, &UserInterface::set_default_font);
        def_property(user_interface, "mouse_cursor_scale", &UserInterface::mouse_cursor_scale, &UserInterface::set_mouse_cursor_scale);
        def_property(user_interface, "anti_aliased_lines", &UserInterface::anti_aliased_lines, &UserInterface::set_anti_aliased_lines);
        def_property(user_interface, "anti_aliased_fill", &UserInterface::anti_aliased_fill, &UserInterface::set_anti_aliased_fill);
        def_property(user_interface, "curve_tessellation_tolerance", &UserInterface::curve_tessellation_tolerance, &UserInterface::set_curve_tessellation_tolerance);
        def_property(user_interface, "circle_tessellation_maximum_error", &UserInterface::circle_tessellation_maximum_error, &UserInterface::set_circle_tessellation_maximum_error);
    }

}
