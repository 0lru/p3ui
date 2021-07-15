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
#include <p3/Theme.h>

#include <imgui.h>
#include <implot.h>


namespace p3::python
{

    void Definition<Theme>::parse(py::kwargs const&, Theme&)
    {
    }

    void Definition<Theme>::apply(py::module& module)
    {
        py::class_<Theme, std::shared_ptr<Theme>>(module, "Theme")
            .def(py::init<>([]() {
            return std::make_shared<Theme>();
        }))
            .def("make_light", &Theme::make_light)
            .def("make_dark", &Theme::make_dark)
            .def("make_classic", &Theme::make_classic)
            .def_property("alpha", &Theme::alpha, &Theme::set_alpha)
            .def_property("window_padding", &Theme::window_padding, &Theme::set_window_padding)
            .def_property("window_rounding", &Theme::window_rounding, &Theme::set_window_rounding)
            .def_property("window_border_size", &Theme::window_border_size, &Theme::set_window_border_size)
            .def_property("window_min_size", &Theme::window_min_size, &Theme::set_window_min_size)
            .def_property("window_title_align", &Theme::window_title_align, &Theme::set_window_title_align)
            .def_property("child_rounding", &Theme::child_rounding, &Theme::set_child_rounding)
            .def_property("child_border_size", &Theme::child_border_size, &Theme::set_child_border_size)
            .def_property("popup_rounding", &Theme::popup_rounding, &Theme::set_popup_rounding)
            .def_property("popup_border_size", &Theme::popup_border_size, &Theme::set_popup_border_size)
            .def_property("frame_padding", &Theme::frame_padding, &Theme::set_frame_padding)
            .def_property("frame_rounding", &Theme::frame_rounding, &Theme::set_frame_rounding)
            .def_property("frame_border_size", &Theme::frame_border_size, &Theme::set_frame_border_size)
            .def_property("item_spacing", &Theme::item_spacing, &Theme::set_item_spacing)
            .def_property("item_inner_spacing", &Theme::item_inner_spacing, &Theme::set_item_inner_spacing)
            .def_property("indent_spacing", &Theme::indent_spacing, &Theme::set_indent_spacing)
            .def_property("cell_padding", &Theme::cell_padding, &Theme::set_cell_padding)
            .def_property("scrollbar_size", &Theme::scrollbar_size, &Theme::set_scrollbar_size)
            .def_property("scrollbar_rounding", &Theme::scrollbar_rounding, &Theme::set_scrollbar_rounding)
            .def_property("grab_min_size", &Theme::grab_min_size, &Theme::set_grab_min_size)
            .def_property("grab_rounding", &Theme::grab_rounding, &Theme::set_grab_rounding)
            .def_property("tab_rounding", &Theme::tab_rounding, &Theme::set_tab_rounding)
            .def_property("button_text_align", &Theme::button_text_align, &Theme::set_button_text_align)
            .def_property("selectable_text_align", &Theme::selectable_text_align, &Theme::set_selectable_text_align)
            .def_property("text_color", &Theme::text_color, &Theme::set_text_color)
            .def_property("text_disabled_color", &Theme::text_disabled_color, &Theme::set_text_disabled_color)
            .def_property("window_background_color", &Theme::window_background_color, &Theme::set_window_background_color)
            .def_property("child_background_color", &Theme::child_background_color, &Theme::set_child_background_color)
            .def_property("popup_background_color", &Theme::popup_background_color, &Theme::set_popup_background_color)
            .def_property("border_color", &Theme::border_color, &Theme::set_border_color)
            .def_property("border_shadow_color", &Theme::border_shadow_color, &Theme::set_border_shadow_color)
            .def_property("frame_background_color", &Theme::frame_background_color, &Theme::set_frame_background_color)
            .def_property("frame_background_hovered_color", &Theme::frame_background_hovered_color, &Theme::set_frame_background_hovered_color)
            .def_property("frame_background_active_color", &Theme::frame_background_active_color, &Theme::set_frame_background_active_color)
            .def_property("title_background_color", &Theme::title_background_color, &Theme::set_title_background_color)
            .def_property("title_background_active_color", &Theme::title_background_active_color, &Theme::set_title_background_active_color)
            .def_property("title_background_collapsed_color", &Theme::title_background_collapsed_color, &Theme::set_title_background_collapsed_color)
            .def_property("menu_bar_background_color", &Theme::menu_bar_background_color, &Theme::set_menu_bar_background_color)
            .def_property("scrollbar_background_color", &Theme::scrollbar_background_color, &Theme::set_scrollbar_background_color)
            .def_property("scrollbar_grab_color", &Theme::scrollbar_grab_color, &Theme::set_scrollbar_grab_color)
            .def_property("scrollbar_grab_hovered_color", &Theme::scrollbar_grab_hovered_color, &Theme::set_scrollbar_grab_hovered_color)
            .def_property("scrollbar_grab_active_color", &Theme::scrollbar_grab_active_color, &Theme::set_scrollbar_grab_active_color)
            .def_property("check_mark_color", &Theme::check_mark_color, &Theme::set_check_mark_color)
            .def_property("slider_grab_color", &Theme::slider_grab_color, &Theme::set_slider_grab_color)
            .def_property("slider_grab_active_color", &Theme::slider_grab_active_color, &Theme::set_slider_grab_active_color)
            .def_property("button_color", &Theme::button_color, &Theme::set_button_color)
            .def_property("button_hovered_color", &Theme::button_hovered_color, &Theme::set_button_hovered_color)
            .def_property("button_active_color", &Theme::button_active_color, &Theme::set_button_active_color)
            .def_property("header_color", &Theme::header_color, &Theme::set_header_color)
            .def_property("header_hovered_color", &Theme::header_hovered_color, &Theme::set_header_hovered_color)
            .def_property("header_active_color", &Theme::header_active_color, &Theme::set_header_active_color)
            .def_property("separator_color", &Theme::separator_color, &Theme::set_separator_color)
            .def_property("separator_hovered_color", &Theme::separator_hovered_color, &Theme::set_separator_hovered_color)
            .def_property("separator_active_color", &Theme::separator_active_color, &Theme::set_separator_active_color)
            .def_property("resize_grip_color", &Theme::resize_grip_color, &Theme::set_resize_grip_color)
            .def_property("resize_grip_hovered_color", &Theme::resize_grip_hovered_color, &Theme::set_resize_grip_hovered_color)
            .def_property("resize_grip_active_color", &Theme::resize_grip_active_color, &Theme::set_resize_grip_active_color)
            .def_property("tab_color", &Theme::tab_color, &Theme::set_tab_color)
            .def_property("tab_hovered_color", &Theme::tab_hovered_color, &Theme::set_tab_hovered_color)
            .def_property("tab_active_color", &Theme::tab_active_color, &Theme::set_tab_active_color)
            .def_property("tab_unfocused_color", &Theme::tab_unfocused_color, &Theme::set_tab_unfocused_color)
            .def_property("tab_unfocused_active_color", &Theme::tab_unfocused_active_color, &Theme::set_tab_unfocused_active_color)
            .def_property("table_header_background_color", &Theme::table_header_background_color, &Theme::set_table_header_background_color)
            .def_property("table_border_strong_color", &Theme::table_border_strong_color, &Theme::set_table_border_strong_color)
            .def_property("table_border_light_color", &Theme::table_border_light_color, &Theme::set_table_border_light_color)
            .def_property("table_row_background_color", &Theme::table_row_background_color, &Theme::set_table_row_background_color)
            .def_property("table_row_background_alt_color", &Theme::table_row_background_alt_color, &Theme::set_table_row_background_alt_color)
            .def_property("text_selected_background_color", &Theme::text_selected_background_color, &Theme::set_text_selected_background_color)
            .def_property("drag_drop_target_color", &Theme::drag_drop_target_color, &Theme::set_drag_drop_target_color)
            .def_property("nav_highlight_color", &Theme::nav_highlight_color, &Theme::set_nav_highlight_color)
            .def_property("nav_windowing_highlight_color", &Theme::nav_windowing_highlight_color, &Theme::set_nav_windowing_highlight_color)
            .def_property("nav_windowing_dim_background_color", &Theme::nav_windowing_dim_background_color, &Theme::set_nav_windowing_dim_background_color)
            .def_property("modal_window_dim_background_color", &Theme::modal_window_dim_background_color, &Theme::set_modal_window_dim_background_color);

    }

}
