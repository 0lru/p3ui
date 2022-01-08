#include "p3ui.h"

#include <p3/Theme.h>

#include <imgui.h>
#include <implot.h>


namespace p3::python
{

    void Definition<Theme>::apply(py::module& module)
    {
        py::class_<Theme, std::shared_ptr<Theme>> theme(module, "Theme");

        theme.def(py::init<>([]() {
            return std::make_shared<Theme>();
        }));

        def_method(theme, "make_light", &Theme::make_light);
        def_method(theme, "make_dark", &Theme::make_dark);
        def_method(theme, "make_classic", &Theme::make_classic);

        def_property(theme, "alpha", &Theme::alpha, &Theme::set_alpha);
        def_property(theme, "window_padding", &Theme::window_padding, &Theme::set_window_padding);
        def_property(theme, "window_rounding", &Theme::window_rounding, &Theme::set_window_rounding);
        def_property(theme, "window_border_size", &Theme::window_border_size, &Theme::set_window_border_size);
        def_property(theme, "window_min_size", &Theme::window_min_size, &Theme::set_window_min_size);
        def_property(theme, "window_title_align", &Theme::window_title_align, &Theme::set_window_title_align);
        def_property(theme, "child_rounding", &Theme::child_rounding, &Theme::set_child_rounding);
        def_property(theme, "child_border_size", &Theme::child_border_size, &Theme::set_child_border_size);
        def_property(theme, "popup_rounding", &Theme::popup_rounding, &Theme::set_popup_rounding);
        def_property(theme, "popup_border_size", &Theme::popup_border_size, &Theme::set_popup_border_size);
        def_property(theme, "frame_padding", &Theme::frame_padding, &Theme::set_frame_padding);
        def_property(theme, "frame_rounding", &Theme::frame_rounding, &Theme::set_frame_rounding);
        def_property(theme, "frame_border_size", &Theme::frame_border_size, &Theme::set_frame_border_size);
        def_property(theme, "item_spacing", &Theme::item_spacing, &Theme::set_item_spacing);
        def_property(theme, "item_inner_spacing", &Theme::item_inner_spacing, &Theme::set_item_inner_spacing);
        def_property(theme, "indent_spacing", &Theme::indent_spacing, &Theme::set_indent_spacing);
        def_property(theme, "cell_padding", &Theme::cell_padding, &Theme::set_cell_padding);
        def_property(theme, "scrollbar_size", &Theme::scrollbar_size, &Theme::set_scrollbar_size);
        def_property(theme, "scrollbar_rounding", &Theme::scrollbar_rounding, &Theme::set_scrollbar_rounding);
        def_property(theme, "grab_min_size", &Theme::grab_min_size, &Theme::set_grab_min_size);
        def_property(theme, "grab_rounding", &Theme::grab_rounding, &Theme::set_grab_rounding);
        def_property(theme, "tab_rounding", &Theme::tab_rounding, &Theme::set_tab_rounding);
        def_property(theme, "button_text_align", &Theme::button_text_align, &Theme::set_button_text_align);
        def_property(theme, "selectable_text_align", &Theme::selectable_text_align, &Theme::set_selectable_text_align);
        def_property(theme, "text_color", &Theme::text_color, &Theme::set_text_color);
        def_property(theme, "text_disabled_color", &Theme::text_disabled_color, &Theme::set_text_disabled_color);
        def_property(theme, "window_background_color", &Theme::window_background_color, &Theme::set_window_background_color);
        def_property(theme, "child_background_color", &Theme::child_background_color, &Theme::set_child_background_color);
        def_property(theme, "popup_background_color", &Theme::popup_background_color, &Theme::set_popup_background_color);
        def_property(theme, "border_color", &Theme::border_color, &Theme::set_border_color);
        def_property(theme, "border_shadow_color", &Theme::border_shadow_color, &Theme::set_border_shadow_color);
        def_property(theme, "frame_background_color", &Theme::frame_background_color, &Theme::set_frame_background_color);
        def_property(theme, "frame_background_hovered_color", &Theme::frame_background_hovered_color, &Theme::set_frame_background_hovered_color);
        def_property(theme, "frame_background_active_color", &Theme::frame_background_active_color, &Theme::set_frame_background_active_color);
        def_property(theme, "title_background_color", &Theme::title_background_color, &Theme::set_title_background_color);
        def_property(theme, "title_background_active_color", &Theme::title_background_active_color, &Theme::set_title_background_active_color);
        def_property(theme, "title_background_collapsed_color", &Theme::title_background_collapsed_color, &Theme::set_title_background_collapsed_color);
        def_property(theme, "menu_bar_background_color", &Theme::menu_bar_background_color, &Theme::set_menu_bar_background_color);
        def_property(theme, "scrollbar_background_color", &Theme::scrollbar_background_color, &Theme::set_scrollbar_background_color);
        def_property(theme, "scrollbar_grab_color", &Theme::scrollbar_grab_color, &Theme::set_scrollbar_grab_color);
        def_property(theme, "scrollbar_grab_hovered_color", &Theme::scrollbar_grab_hovered_color, &Theme::set_scrollbar_grab_hovered_color);
        def_property(theme, "scrollbar_grab_active_color", &Theme::scrollbar_grab_active_color, &Theme::set_scrollbar_grab_active_color);
        def_property(theme, "check_mark_color", &Theme::check_mark_color, &Theme::set_check_mark_color);
        def_property(theme, "slider_grab_color", &Theme::slider_grab_color, &Theme::set_slider_grab_color);
        def_property(theme, "slider_grab_active_color", &Theme::slider_grab_active_color, &Theme::set_slider_grab_active_color);
        def_property(theme, "button_color", &Theme::button_color, &Theme::set_button_color);
        def_property(theme, "button_hovered_color", &Theme::button_hovered_color, &Theme::set_button_hovered_color);
        def_property(theme, "button_active_color", &Theme::button_active_color, &Theme::set_button_active_color);
        def_property(theme, "header_color", &Theme::header_color, &Theme::set_header_color);
        def_property(theme, "progress_bar_color", &Theme::progress_bar_color, &Theme::set_progress_bar_color);
        def_property(theme, "header_hovered_color", &Theme::header_hovered_color, &Theme::set_header_hovered_color);
        def_property(theme, "header_active_color", &Theme::header_active_color, &Theme::set_header_active_color);
        def_property(theme, "separator_color", &Theme::separator_color, &Theme::set_separator_color);
        def_property(theme, "separator_hovered_color", &Theme::separator_hovered_color, &Theme::set_separator_hovered_color);
        def_property(theme, "separator_active_color", &Theme::separator_active_color, &Theme::set_separator_active_color);
        def_property(theme, "resize_grip_color", &Theme::resize_grip_color, &Theme::set_resize_grip_color);
        def_property(theme, "resize_grip_hovered_color", &Theme::resize_grip_hovered_color, &Theme::set_resize_grip_hovered_color);
        def_property(theme, "resize_grip_active_color", &Theme::resize_grip_active_color, &Theme::set_resize_grip_active_color);
        def_property(theme, "tab_color", &Theme::tab_color, &Theme::set_tab_color);
        def_property(theme, "tab_hovered_color", &Theme::tab_hovered_color, &Theme::set_tab_hovered_color);
        def_property(theme, "tab_active_color", &Theme::tab_active_color, &Theme::set_tab_active_color);
        def_property(theme, "tab_unfocused_color", &Theme::tab_unfocused_color, &Theme::set_tab_unfocused_color);
        def_property(theme, "tab_unfocused_active_color", &Theme::tab_unfocused_active_color, &Theme::set_tab_unfocused_active_color);
        def_property(theme, "table_header_background_color", &Theme::table_header_background_color, &Theme::set_table_header_background_color);
        def_property(theme, "table_border_strong_color", &Theme::table_border_strong_color, &Theme::set_table_border_strong_color);
        def_property(theme, "table_border_light_color", &Theme::table_border_light_color, &Theme::set_table_border_light_color);
        def_property(theme, "table_row_background_color", &Theme::table_row_background_color, &Theme::set_table_row_background_color);
        def_property(theme, "table_row_background_alt_color", &Theme::table_row_background_alt_color, &Theme::set_table_row_background_alt_color);
        def_property(theme, "text_selected_background_color", &Theme::text_selected_background_color, &Theme::set_text_selected_background_color);
        def_property(theme, "drag_drop_target_color", &Theme::drag_drop_target_color, &Theme::set_drag_drop_target_color);
        def_property(theme, "nav_highlight_color", &Theme::nav_highlight_color, &Theme::set_nav_highlight_color);
        def_property(theme, "nav_windowing_highlight_color", &Theme::nav_windowing_highlight_color, &Theme::set_nav_windowing_highlight_color);
        def_property(theme, "nav_windowing_dim_background_color", &Theme::nav_windowing_dim_background_color, &Theme::set_nav_windowing_dim_background_color);
        def_property(theme, "modal_window_dim_background_color", &Theme::modal_window_dim_background_color, &Theme::set_modal_window_dim_background_color);
        // plot
        def_property(theme, "plot_line_color", &Theme::plot_line_color, &Theme::set_plot_line_color);
        def_property(theme, "plot_line_weight", &Theme::plot_line_weight, &Theme::set_plot_line_weight);
        def_property(theme, "plot_fill_color", &Theme::plot_fill_color, &Theme::set_plot_fill_color);
        def_property(theme, "plot_marker_outline_color", &Theme::plot_marker_outline_color, &Theme::set_plot_marker_outline_color);
        def_property(theme, "plot_marker_fill_color", &Theme::plot_marker_fill_color, &Theme::set_plot_marker_fill_color);
        def_property(theme, "plot_error_bar_color", &Theme::plot_error_bar_color, &Theme::set_plot_error_bar_color);
        def_property(theme, "plot_frame_background_color", &Theme::plot_frame_background_color, &Theme::set_plot_frame_background_color);
        def_property(theme, "plot_background_color", &Theme::plot_background_color, &Theme::set_plot_background_color);
        def_property(theme, "plot_border_color", &Theme::plot_border_color, &Theme::set_plot_border_color);
        def_property(theme, "plot_legend_background_color", &Theme::plot_legend_background_color, &Theme::set_plot_legend_background_color);
        def_property(theme, "plot_legend_border_color", &Theme::plot_legend_border_color, &Theme::set_plot_legend_border_color);
        def_property(theme, "plot_legend_text_color", &Theme::plot_legend_text_color, &Theme::set_plot_legend_text_color);
        def_property(theme, "plot_title_text_color", &Theme::plot_title_text_color, &Theme::set_plot_title_text_color);
        def_property(theme, "plot_inlay_text_color", &Theme::plot_inlay_text_color, &Theme::set_plot_inlay_text_color);
        def_property(theme, "plot_axis_color", &Theme::plot_axis_color, &Theme::set_plot_axis_color);
        def_property(theme, "plot_axis_grid_color", &Theme::plot_axis_grid_color, &Theme::set_plot_axis_grid_color);
        def_property(theme, "plot_selection_color", &Theme::plot_selection_color, &Theme::set_plot_selection_color);
        def_property(theme, "plot_crosshairs_color", &Theme::plot_crosshairs_color, &Theme::set_plot_crosshairs_color);
    }

}
