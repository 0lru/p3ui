#include "Theme.h"
#include "Context.h"
#include "convert.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <implot_internal.h>

namespace p3 {

namespace {

    ImVec2 to_actual(Context const& context, Length2 const& length)
    {
        return ImVec2(context.to_actual(length[0]), context.to_actual(length[1]));
    }

    ImVec2 to_actual(Context const& context, Float2 const& value)
    {
        return ImVec2(value[0], value[1]);
    }

}

void Theme::add_observer(Observer* observer)
{
    _observer.push_back(observer);
}

void Theme::remove_observer(Observer* observer)
{
    _observer.erase(std::remove_if(_observer.begin(), _observer.end(), [&](auto item) {
        return item == observer;
    }),
        _observer.end());
}

Theme::ApplyFunction Theme::compile(Context const& context)
{
    auto im_gui_style = std::make_shared<ImGuiStyle>();
    im_gui_style->Alpha = _alpha;
    im_gui_style->WindowPadding = to_actual(context, _window_padding);
    im_gui_style->WindowRounding = context.to_actual(_window_rounding);
    im_gui_style->WindowBorderSize = context.to_actual(_window_border_size);
    im_gui_style->WindowMinSize = to_actual(context, _window_min_size);
    im_gui_style->WindowTitleAlign = to_actual(context, _window_title_align);
    im_gui_style->ChildRounding = context.to_actual(_child_rounding);
    im_gui_style->ChildBorderSize = context.to_actual(_child_border_size);
    im_gui_style->PopupRounding = context.to_actual(_popup_rounding);
    im_gui_style->PopupBorderSize = context.to_actual(_popup_border_size);
    im_gui_style->FramePadding = to_actual(context, _frame_padding);
    im_gui_style->FrameRounding = context.to_actual(_frame_rounding);
    im_gui_style->FrameBorderSize = context.to_actual(_frame_border_size);
    im_gui_style->ItemSpacing = to_actual(context, _item_spacing);
    im_gui_style->ItemInnerSpacing = to_actual(context, _item_inner_spacing);
    im_gui_style->IndentSpacing = context.to_actual(_indent_spacing);
    im_gui_style->CellPadding = to_actual(context, _cell_padding);
    im_gui_style->ScrollbarSize = context.to_actual(_scrollbar_size);
    im_gui_style->ScrollbarRounding = context.to_actual(_scrollbar_rounding);
    im_gui_style->GrabMinSize = context.to_actual(_grab_min_size);
    im_gui_style->GrabRounding = context.to_actual(_grab_rounding);
    im_gui_style->TabRounding = context.to_actual(_tab_rounding);
    im_gui_style->ButtonTextAlign = to_actual(context, _button_text_align);
    im_gui_style->SelectableTextAlign = to_actual(context, _selectable_text_align);
    assign(im_gui_style->Colors[ImGuiCol_Text], _text_color);
    assign(im_gui_style->Colors[ImGuiCol_TextDisabled], _text_disabled_color);
    assign(im_gui_style->Colors[ImGuiCol_WindowBg], _window_background_color);
    assign(im_gui_style->Colors[ImGuiCol_ChildBg], _child_background_color);
    assign(im_gui_style->Colors[ImGuiCol_PopupBg], _popup_background_color);
    assign(im_gui_style->Colors[ImGuiCol_Border], _border_color);
    assign(im_gui_style->Colors[ImGuiCol_BorderShadow], _border_shadow_color);
    assign(im_gui_style->Colors[ImGuiCol_FrameBg], _frame_background_color);
    assign(im_gui_style->Colors[ImGuiCol_FrameBgHovered], _frame_background_hovered_color);
    assign(im_gui_style->Colors[ImGuiCol_FrameBgActive], _frame_background_active_color);
    assign(im_gui_style->Colors[ImGuiCol_TitleBg], _title_background_color);
    assign(im_gui_style->Colors[ImGuiCol_TitleBgActive], _title_background_active_color);
    assign(im_gui_style->Colors[ImGuiCol_TitleBgCollapsed], _title_background_collapsed_color);
    assign(im_gui_style->Colors[ImGuiCol_MenuBarBg], _menu_bar_background_color);
    assign(im_gui_style->Colors[ImGuiCol_ScrollbarBg], _scrollbar_background_color);
    assign(im_gui_style->Colors[ImGuiCol_ScrollbarGrab], _scrollbar_grab_color);
    assign(im_gui_style->Colors[ImGuiCol_ScrollbarGrabHovered], _scrollbar_grab_hovered_color);
    assign(im_gui_style->Colors[ImGuiCol_ScrollbarGrabActive], _scrollbar_grab_active_color);
    assign(im_gui_style->Colors[ImGuiCol_CheckMark], _check_mark_color);
    assign(im_gui_style->Colors[ImGuiCol_SliderGrab], _slider_grab_color);
    assign(im_gui_style->Colors[ImGuiCol_SliderGrabActive], _slider_grab_active_color);
    assign(im_gui_style->Colors[ImGuiCol_Button], _button_color);
    assign(im_gui_style->Colors[ImGuiCol_ButtonHovered], _button_hovered_color);
    assign(im_gui_style->Colors[ImGuiCol_ButtonActive], _button_active_color);
    assign(im_gui_style->Colors[ImGuiCol_Header], _header_color);
    assign(im_gui_style->Colors[ImGuiCol_PlotHistogram], _progress_bar_color);
    assign(im_gui_style->Colors[ImGuiCol_HeaderHovered], _header_hovered_color);
    assign(im_gui_style->Colors[ImGuiCol_HeaderActive], _header_active_color);
    assign(im_gui_style->Colors[ImGuiCol_Separator], _separator_color);
    assign(im_gui_style->Colors[ImGuiCol_SeparatorHovered], _separator_hovered_color);
    assign(im_gui_style->Colors[ImGuiCol_SeparatorActive], _separator_active_color);
    assign(im_gui_style->Colors[ImGuiCol_ResizeGrip], _resize_grip_color);
    assign(im_gui_style->Colors[ImGuiCol_ResizeGripHovered], _resize_grip_hovered_color);
    assign(im_gui_style->Colors[ImGuiCol_ResizeGripActive], _resize_grip_active_color);
    assign(im_gui_style->Colors[ImGuiCol_Tab], _tab_color);
    assign(im_gui_style->Colors[ImGuiCol_TabHovered], _tab_hovered_color);
    assign(im_gui_style->Colors[ImGuiCol_TabActive], _tab_active_color);
    assign(im_gui_style->Colors[ImGuiCol_TabUnfocused], _tab_unfocused_color);
    assign(im_gui_style->Colors[ImGuiCol_TabUnfocusedActive], _tab_unfocused_active_color);
    assign(im_gui_style->Colors[ImGuiCol_TableHeaderBg], _table_header_background_color);
    assign(im_gui_style->Colors[ImGuiCol_TableBorderStrong], _table_border_strong_color);
    assign(im_gui_style->Colors[ImGuiCol_TableBorderLight], _table_border_light_color);
    assign(im_gui_style->Colors[ImGuiCol_TableRowBg], _table_row_background_color);
    assign(im_gui_style->Colors[ImGuiCol_TableRowBgAlt], _table_row_background_alt_color);
    assign(im_gui_style->Colors[ImGuiCol_TextSelectedBg], _text_selected_background_color);
    assign(im_gui_style->Colors[ImGuiCol_DragDropTarget], _drag_drop_target_color);
    assign(im_gui_style->Colors[ImGuiCol_NavHighlight], _nav_highlight_color);
    assign(im_gui_style->Colors[ImGuiCol_NavWindowingHighlight], _nav_windowing_highlight_color);
    assign(im_gui_style->Colors[ImGuiCol_NavWindowingDimBg], _nav_windowing_dim_background_color);
    assign(im_gui_style->Colors[ImGuiCol_ModalWindowDimBg], _modal_window_dim_background_color);

    auto im_plot_style = std::make_shared<ImPlotStyle>();
    assign(im_plot_style->Colors[ImPlotCol_Line], _plot_line_color);
    assign(im_plot_style->Colors[ImPlotCol_Fill], _plot_fill_color);
    assign(im_plot_style->Colors[ImPlotCol_MarkerOutline], _plot_marker_outline_color);
    assign(im_plot_style->Colors[ImPlotCol_MarkerFill], _plot_marker_fill_color);
    assign(im_plot_style->Colors[ImPlotCol_ErrorBar], _plot_error_bar_color);
    assign(im_plot_style->Colors[ImPlotCol_FrameBg], _plot_frame_background_color);
    assign(im_plot_style->Colors[ImPlotCol_PlotBg], _plot_background_color);
    assign(im_plot_style->Colors[ImPlotCol_PlotBorder], _plot_border_color);
    assign(im_plot_style->Colors[ImPlotCol_LegendBg], _plot_legend_background_color);
    assign(im_plot_style->Colors[ImPlotCol_LegendBorder], _plot_legend_border_color);
    assign(im_plot_style->Colors[ImPlotCol_LegendText], _plot_legend_text_color);
    assign(im_plot_style->Colors[ImPlotCol_TitleText], _plot_title_text_color);
    assign(im_plot_style->Colors[ImPlotCol_InlayText], _plot_inlay_text_color);
    assign(im_plot_style->Colors[ImPlotCol_AxisText], _plot_axis_color);
    assign(im_plot_style->Colors[ImPlotCol_AxisGrid], _plot_axis_grid_color);
    assign(im_plot_style->Colors[ImPlotCol_Selection], _plot_selection_color);
    assign(im_plot_style->Colors[ImPlotCol_Crosshairs], _plot_crosshairs_color);
    im_plot_style->LineWeight = _plot_line_weight;
    im_gui_style->AntiAliasedLines = true;
    im_gui_style->AntiAliasedFill = true;
    auto apply = [im_gui_style { std::move(im_gui_style) }, im_plot_style { std::move(im_plot_style) }]() mutable {
        if (GImGui)
            std::swap(*im_gui_style, GImGui->Style);
        if (GImPlot)
            std::swap(*im_plot_style, GImPlot->Style);
    };

    return [apply]() mutable {
        apply();
        return OnScopeExit(apply);
    };
}

float Theme::alpha() const
{
    return _alpha;
}

void Theme::set_alpha(float alpha)
{
    _alpha = std::move(alpha);
    _on_change();
}

Length2 Theme::window_padding() const
{
    return _window_padding;
}

void Theme::set_window_padding(Length2 window_padding)
{
    _window_padding = std::move(window_padding);
    _on_change();
}

Length Theme::window_rounding() const
{
    return _window_rounding;
}

void Theme::set_window_rounding(Length window_rounding)
{
    _window_rounding = std::move(window_rounding);
    _on_change();
}

Length Theme::window_border_size() const
{
    return _window_border_size;
}

void Theme::set_window_border_size(Length window_border_size)
{
    _window_border_size = std::move(window_border_size);
    _on_change();
}

Length2 Theme::window_min_size() const
{
    return _window_min_size;
}

void Theme::set_window_min_size(Length2 window_min_size)
{
    _window_min_size = std::move(window_min_size);
    _on_change();
}

Float2 Theme::window_title_align() const
{
    return _window_title_align;
}

void Theme::set_window_title_align(Float2 window_title_align)
{
    _window_title_align = std::move(window_title_align);
    _on_change();
}

Length Theme::child_rounding() const
{
    return _child_rounding;
}

void Theme::set_child_rounding(Length child_rounding)
{
    _child_rounding = std::move(child_rounding);
    _on_change();
}

Length Theme::child_border_size() const
{
    return _child_border_size;
}

void Theme::set_child_border_size(Length child_border_size)
{
    _child_border_size = std::move(child_border_size);
    _on_change();
}

Length Theme::popup_rounding() const
{
    return _popup_rounding;
}

void Theme::set_popup_rounding(Length popup_rounding)
{
    _popup_rounding = std::move(popup_rounding);
    _on_change();
}

Length Theme::popup_border_size() const
{
    return _popup_border_size;
}

void Theme::set_popup_border_size(Length popup_border_size)
{
    _popup_border_size = std::move(popup_border_size);
    _on_change();
}

Length2 Theme::frame_padding() const
{
    return _frame_padding;
}

void Theme::set_frame_padding(Length2 frame_padding)
{
    _frame_padding = std::move(frame_padding);
    _on_change();
}

Length Theme::frame_rounding() const
{
    return _frame_rounding;
}

void Theme::set_frame_rounding(Length frame_rounding)
{
    _frame_rounding = std::move(frame_rounding);
    _on_change();
}

Length Theme::frame_border_size() const
{
    return _frame_border_size;
}

void Theme::set_frame_border_size(Length frame_border_size)
{
    _frame_border_size = std::move(frame_border_size);
    _on_change();
}

Length2 Theme::item_spacing() const
{
    return _item_spacing;
}

void Theme::set_item_spacing(Length2 item_spacing)
{
    _item_spacing = std::move(item_spacing);
    _on_change();
}

Length2 Theme::item_inner_spacing() const
{
    return _item_inner_spacing;
}

void Theme::set_item_inner_spacing(Length2 item_inner_spacing)
{
    _item_inner_spacing = std::move(item_inner_spacing);
    _on_change();
}

Length Theme::indent_spacing() const
{
    return _indent_spacing;
}

void Theme::set_indent_spacing(Length indent_spacing)
{
    _indent_spacing = std::move(indent_spacing);
    _on_change();
}

Length2 Theme::cell_padding() const
{
    return _cell_padding;
}

void Theme::set_cell_padding(Length2 cell_padding)
{
    _cell_padding = std::move(cell_padding);
    _on_change();
}

Length Theme::scrollbar_size() const
{
    return _scrollbar_size;
}

void Theme::set_scrollbar_size(Length scrollbar_size)
{
    _scrollbar_size = std::move(scrollbar_size);
    _on_change();
}

Length Theme::scrollbar_rounding() const
{
    return _scrollbar_rounding;
}

void Theme::set_scrollbar_rounding(Length scrollbar_rounding)
{
    _scrollbar_rounding = std::move(scrollbar_rounding);
    _on_change();
}

Length Theme::grab_min_size() const
{
    return _grab_min_size;
}

void Theme::set_grab_min_size(Length grab_min_size)
{
    _grab_min_size = std::move(grab_min_size);
    _on_change();
}

Length Theme::grab_rounding() const
{
    return _grab_rounding;
}

void Theme::set_grab_rounding(Length grab_rounding)
{
    _grab_rounding = std::move(grab_rounding);
    _on_change();
}

Length Theme::tab_rounding() const
{
    return _tab_rounding;
}

void Theme::set_tab_rounding(Length tab_rounding)
{
    _tab_rounding = std::move(tab_rounding);
    _on_change();
}

Float2 Theme::button_text_align() const
{
    return _button_text_align;
}

void Theme::set_button_text_align(Float2 button_text_align)
{
    _button_text_align = std::move(button_text_align);
    _on_change();
}

Float2 Theme::selectable_text_align() const
{
    return _selectable_text_align;
}

void Theme::set_selectable_text_align(Float2 selectable_text_align)
{
    _selectable_text_align = std::move(selectable_text_align);
    _on_change();
}

Color Theme::text_color() const
{
    return _text_color;
}

void Theme::set_text_color(Color text_color)
{
    _text_color = std::move(text_color);
    _on_change();
}

Color Theme::text_disabled_color() const
{
    return _text_disabled_color;
}

void Theme::set_text_disabled_color(Color text_disabled_color)
{
    _text_disabled_color = std::move(text_disabled_color);
    _on_change();
}

Color Theme::window_background_color() const
{
    return _window_background_color;
}

void Theme::set_window_background_color(Color window_background_color)
{
    _window_background_color = std::move(window_background_color);
    _on_change();
}

Color Theme::child_background_color() const
{
    return _child_background_color;
}

void Theme::set_child_background_color(Color child_background_color)
{
    _child_background_color = std::move(child_background_color);
    _on_change();
}

Color Theme::popup_background_color() const
{
    return _popup_background_color;
}

void Theme::set_popup_background_color(Color popup_background_color)
{
    _popup_background_color = std::move(popup_background_color);
    _on_change();
}

Color Theme::border_color() const
{
    return _border_color;
}

void Theme::set_border_color(Color border_color)
{
    _border_color = std::move(border_color);
    _on_change();
}

Color Theme::border_shadow_color() const
{
    return _border_shadow_color;
}

void Theme::set_border_shadow_color(Color border_shadow_color)
{
    _border_shadow_color = std::move(border_shadow_color);
    _on_change();
}

Color Theme::frame_background_color() const
{
    return _frame_background_color;
}

void Theme::set_frame_background_color(Color frame_background_color)
{
    _frame_background_color = std::move(frame_background_color);
    _on_change();
}

Color Theme::frame_background_hovered_color() const
{
    return _frame_background_hovered_color;
}

void Theme::set_frame_background_hovered_color(Color frame_background_hovered_color)
{
    _frame_background_hovered_color = std::move(frame_background_hovered_color);
    _on_change();
}

Color Theme::frame_background_active_color() const
{
    return _frame_background_active_color;
}

void Theme::set_frame_background_active_color(Color frame_background_active_color)
{
    _frame_background_active_color = std::move(frame_background_active_color);
    _on_change();
}

Color Theme::title_background_color() const
{
    return _title_background_color;
}

void Theme::set_title_background_color(Color title_background_color)
{
    _title_background_color = std::move(title_background_color);
    _on_change();
}

Color Theme::title_background_active_color() const
{
    return _title_background_active_color;
}

void Theme::set_title_background_active_color(Color title_background_active_color)
{
    _title_background_active_color = std::move(title_background_active_color);
    _on_change();
}

Color Theme::title_background_collapsed_color() const
{
    return _title_background_collapsed_color;
}

void Theme::set_title_background_collapsed_color(Color title_background_collapsed_color)
{
    _title_background_collapsed_color = std::move(title_background_collapsed_color);
    _on_change();
}

Color Theme::menu_bar_background_color() const
{
    return _menu_bar_background_color;
}

void Theme::set_menu_bar_background_color(Color menu_bar_background_color)
{
    _menu_bar_background_color = std::move(menu_bar_background_color);
    _on_change();
}

Color Theme::scrollbar_background_color() const
{
    return _scrollbar_background_color;
}

void Theme::set_scrollbar_background_color(Color scrollbar_background_color)
{
    _scrollbar_background_color = std::move(scrollbar_background_color);
    _on_change();
}

Color Theme::scrollbar_grab_color() const
{
    return _scrollbar_grab_color;
}

void Theme::set_scrollbar_grab_color(Color scrollbar_grab_color)
{
    _scrollbar_grab_color = std::move(scrollbar_grab_color);
    _on_change();
}

Color Theme::scrollbar_grab_hovered_color() const
{
    return _scrollbar_grab_hovered_color;
}

void Theme::set_scrollbar_grab_hovered_color(Color scrollbar_grab_hovered_color)
{
    _scrollbar_grab_hovered_color = std::move(scrollbar_grab_hovered_color);
    _on_change();
}

Color Theme::scrollbar_grab_active_color() const
{
    return _scrollbar_grab_active_color;
}

void Theme::set_scrollbar_grab_active_color(Color scrollbar_grab_active_color)
{
    _scrollbar_grab_active_color = std::move(scrollbar_grab_active_color);
    _on_change();
}

Color Theme::check_mark_color() const
{
    return _check_mark_color;
}

void Theme::set_check_mark_color(Color check_mark_color)
{
    _check_mark_color = std::move(check_mark_color);
    _on_change();
}

Color Theme::slider_grab_color() const
{
    return _slider_grab_color;
}

void Theme::set_slider_grab_color(Color slider_grab_color)
{
    _slider_grab_color = std::move(slider_grab_color);
    _on_change();
}

Color Theme::slider_grab_active_color() const
{
    return _slider_grab_active_color;
}

void Theme::set_slider_grab_active_color(Color slider_grab_active_color)
{
    _slider_grab_active_color = std::move(slider_grab_active_color);
    _on_change();
}

Color Theme::button_color() const
{
    return _button_color;
}

void Theme::set_button_color(Color button_color)
{
    _button_color = std::move(button_color);
    _on_change();
}

Color Theme::button_hovered_color() const
{
    return _button_hovered_color;
}

void Theme::set_button_hovered_color(Color button_hovered_color)
{
    _button_hovered_color = std::move(button_hovered_color);
    _on_change();
}

Color Theme::button_active_color() const
{
    return _button_active_color;
}

void Theme::set_button_active_color(Color button_active_color)
{
    _button_active_color = std::move(button_active_color);
    _on_change();
}

Color Theme::header_color() const
{
    return _header_color;
}

void Theme::set_header_color(Color header_color)
{
    _header_color = std::move(header_color);
    _on_change();
}

Color Theme::progress_bar_color() const
{
    return _progress_bar_color;
}

void Theme::set_progress_bar_color(Color progress_bar_color)
{
    _progress_bar_color = std::move(progress_bar_color);
    _on_change();
}

Color Theme::header_hovered_color() const
{
    return _header_hovered_color;
}

void Theme::set_header_hovered_color(Color header_hovered_color)
{
    _header_hovered_color = std::move(header_hovered_color);
    _on_change();
}

Color Theme::header_active_color() const
{
    return _header_active_color;
}

void Theme::set_header_active_color(Color header_active_color)
{
    _header_active_color = std::move(header_active_color);
    _on_change();
}

Color Theme::separator_color() const
{
    return _separator_color;
}

void Theme::set_separator_color(Color separator_color)
{
    _separator_color = std::move(separator_color);
    _on_change();
}

Color Theme::separator_hovered_color() const
{
    return _separator_hovered_color;
}

void Theme::set_separator_hovered_color(Color separator_hovered_color)
{
    _separator_hovered_color = std::move(separator_hovered_color);
    _on_change();
}

Color Theme::separator_active_color() const
{
    return _separator_active_color;
}

void Theme::set_separator_active_color(Color separator_active_color)
{
    _separator_active_color = std::move(separator_active_color);
    _on_change();
}

Color Theme::resize_grip_color() const
{
    return _resize_grip_color;
}

void Theme::set_resize_grip_color(Color resize_grip_color)
{
    _resize_grip_color = std::move(resize_grip_color);
    _on_change();
}

Color Theme::resize_grip_hovered_color() const
{
    return _resize_grip_hovered_color;
}

void Theme::set_resize_grip_hovered_color(Color resize_grip_hovered_color)
{
    _resize_grip_hovered_color = std::move(resize_grip_hovered_color);
    _on_change();
}

Color Theme::resize_grip_active_color() const
{
    return _resize_grip_active_color;
}

void Theme::set_resize_grip_active_color(Color resize_grip_active_color)
{
    _resize_grip_active_color = std::move(resize_grip_active_color);
    _on_change();
}

Color Theme::tab_color() const
{
    return _tab_color;
}

void Theme::set_tab_color(Color tab_color)
{
    _tab_color = std::move(tab_color);
    _on_change();
}

Color Theme::tab_hovered_color() const
{
    return _tab_hovered_color;
}

void Theme::set_tab_hovered_color(Color tab_hovered_color)
{
    _tab_hovered_color = std::move(tab_hovered_color);
    _on_change();
}

Color Theme::tab_active_color() const
{
    return _tab_active_color;
}

void Theme::set_tab_active_color(Color tab_active_color)
{
    _tab_active_color = std::move(tab_active_color);
    _on_change();
}

Color Theme::tab_unfocused_color() const
{
    return _tab_unfocused_color;
}

void Theme::set_tab_unfocused_color(Color tab_unfocused_color)
{
    _tab_unfocused_color = std::move(tab_unfocused_color);
    _on_change();
}

Color Theme::tab_unfocused_active_color() const
{
    return _tab_unfocused_active_color;
}

void Theme::set_tab_unfocused_active_color(Color tab_unfocused_active_color)
{
    _tab_unfocused_active_color = std::move(tab_unfocused_active_color);
    _on_change();
}

Color Theme::table_header_background_color() const
{
    return _table_header_background_color;
}

void Theme::set_table_header_background_color(Color table_header_background_color)
{
    _table_header_background_color = std::move(table_header_background_color);
    _on_change();
}

Color Theme::table_border_strong_color() const
{
    return _table_border_strong_color;
}

void Theme::set_table_border_strong_color(Color table_border_strong_color)
{
    _table_border_strong_color = std::move(table_border_strong_color);
    _on_change();
}

Color Theme::table_border_light_color() const
{
    return _table_border_light_color;
}

void Theme::set_table_border_light_color(Color table_border_light_color)
{
    _table_border_light_color = std::move(table_border_light_color);
    _on_change();
}

Color Theme::table_row_background_color() const
{
    return _table_row_background_color;
}

void Theme::set_table_row_background_color(Color table_row_background_color)
{
    _table_row_background_color = std::move(table_row_background_color);
    _on_change();
}

Color Theme::table_row_background_alt_color() const
{
    return _table_row_background_alt_color;
}

void Theme::set_table_row_background_alt_color(Color table_row_background_alt_color)
{
    _table_row_background_alt_color = std::move(table_row_background_alt_color);
    _on_change();
}

Color Theme::text_selected_background_color() const
{
    return _text_selected_background_color;
}

void Theme::set_text_selected_background_color(Color text_selected_background_color)
{
    _text_selected_background_color = std::move(text_selected_background_color);
    _on_change();
}

Color Theme::drag_drop_target_color() const
{
    return _drag_drop_target_color;
}

void Theme::set_drag_drop_target_color(Color drag_drop_target_color)
{
    _drag_drop_target_color = std::move(drag_drop_target_color);
    _on_change();
}

Color Theme::nav_highlight_color() const
{
    return _nav_highlight_color;
}

void Theme::set_nav_highlight_color(Color nav_highlight_color)
{
    _nav_highlight_color = std::move(nav_highlight_color);
    _on_change();
}

Color Theme::nav_windowing_highlight_color() const
{
    return _nav_windowing_highlight_color;
}

void Theme::set_nav_windowing_highlight_color(Color nav_windowing_highlight_color)
{
    _nav_windowing_highlight_color = std::move(nav_windowing_highlight_color);
    _on_change();
}

Color Theme::nav_windowing_dim_background_color() const
{
    return _nav_windowing_dim_background_color;
}

void Theme::set_nav_windowing_dim_background_color(Color nav_windowing_dim_background_color)
{
    _nav_windowing_dim_background_color = std::move(nav_windowing_dim_background_color);
    _on_change();
}

Color Theme::modal_window_dim_background_color() const
{
    return _modal_window_dim_background_color;
}

void Theme::set_modal_window_dim_background_color(Color modal_window_dim_background_color)
{
    _modal_window_dim_background_color = std::move(modal_window_dim_background_color);
    _on_change();
}

std::optional<Color> Theme::plot_line_color() const
{
    return _plot_line_color;
}

void Theme::set_plot_line_color(std::optional<Color> plot_line_color)
{
    _plot_line_color = std::move(plot_line_color);
    _on_change();
}

float Theme::plot_line_weight() const
{
    return _plot_line_weight;
}

void Theme::set_plot_line_weight(float line_weight)
{
    _plot_line_weight = line_weight;
}

std::optional<Color> Theme::plot_fill_color() const
{
    return _plot_fill_color;
}

void Theme::set_plot_fill_color(std::optional<Color> plot_fill_color)
{
    _plot_fill_color = std::move(plot_fill_color);
    _on_change();
}

std::optional<Color> Theme::plot_marker_outline_color() const
{
    return _plot_marker_outline_color;
}

void Theme::set_plot_marker_outline_color(std::optional<Color> plot_marker_outline_color)
{
    _plot_marker_outline_color = std::move(plot_marker_outline_color);
    _on_change();
}

std::optional<Color> Theme::plot_marker_fill_color() const
{
    return _plot_marker_fill_color;
}

void Theme::set_plot_marker_fill_color(std::optional<Color> plot_marker_fill_color)
{
    _plot_marker_fill_color = std::move(plot_marker_fill_color);
    _on_change();
}

std::optional<Color> Theme::plot_error_bar_color() const
{
    return _plot_error_bar_color;
}

void Theme::set_plot_error_bar_color(std::optional<Color> plot_error_bar_color)
{
    _plot_error_bar_color = std::move(plot_error_bar_color);
    _on_change();
}

std::optional<Color> Theme::plot_frame_background_color() const
{
    return _plot_frame_background_color;
}

void Theme::set_plot_frame_background_color(std::optional<Color> plot_frame_background_color)
{
    _plot_frame_background_color = std::move(plot_frame_background_color);
    _on_change();
}

std::optional<Color> Theme::plot_background_color() const
{
    return _plot_background_color;
}

void Theme::set_plot_background_color(std::optional<Color> plot_background_color)
{
    _plot_background_color = std::move(plot_background_color);
    _on_change();
}

std::optional<Color> Theme::plot_border_color() const
{
    return _plot_border_color;
}

void Theme::set_plot_border_color(std::optional<Color> plot_border_color)
{
    _plot_border_color = std::move(plot_border_color);
    _on_change();
}

std::optional<Color> Theme::plot_legend_background_color() const
{
    return _plot_legend_background_color;
}

void Theme::set_plot_legend_background_color(std::optional<Color> plot_legend_background_color)
{
    _plot_legend_background_color = std::move(plot_legend_background_color);
    _on_change();
}

std::optional<Color> Theme::plot_legend_border_color() const
{
    return _plot_legend_border_color;
}

void Theme::set_plot_legend_border_color(std::optional<Color> plot_legend_border_color)
{
    _plot_legend_border_color = std::move(plot_legend_border_color);
    _on_change();
}

std::optional<Color> Theme::plot_legend_text_color() const
{
    return _plot_legend_text_color;
}

void Theme::set_plot_legend_text_color(std::optional<Color> plot_legend_text_color)
{
    _plot_legend_text_color = std::move(plot_legend_text_color);
    _on_change();
}

std::optional<Color> Theme::plot_title_text_color() const
{
    return _plot_title_text_color;
}

void Theme::set_plot_title_text_color(std::optional<Color> plot_title_text_color)
{
    _plot_title_text_color = std::move(plot_title_text_color);
    _on_change();
}

std::optional<Color> Theme::plot_inlay_text_color() const
{
    return _plot_inlay_text_color;
}

void Theme::set_plot_inlay_text_color(std::optional<Color> plot_inlay_text_color)
{
    _plot_inlay_text_color = std::move(plot_inlay_text_color);
    _on_change();
}

std::optional<Color> Theme::plot_axis_color() const
{
    return _plot_axis_color;
}

void Theme::set_plot_axis_color(std::optional<Color> plot_x_axis_color)
{
    _plot_axis_color = std::move(plot_x_axis_color);
    _on_change();
}

std::optional<Color> Theme::plot_axis_grid_color() const
{
    return _plot_axis_grid_color;
}

void Theme::set_plot_axis_grid_color(std::optional<Color> plot_x_axis_grid_color)
{
    _plot_axis_grid_color = std::move(plot_x_axis_grid_color);
    _on_change();
}

std::optional<Color> Theme::plot_selection_color() const
{
    return _plot_selection_color;
}

void Theme::set_plot_selection_color(std::optional<Color> plot_selection_color)
{
    _plot_selection_color = std::move(plot_selection_color);
    _on_change();
}

std::optional<Color> Theme::plot_crosshairs_color() const
{
    return _plot_crosshairs_color;
}

void Theme::set_plot_crosshairs_color(std::optional<Color> plot_crosshairs_color)
{
    _plot_crosshairs_color = std::move(plot_crosshairs_color);
    _on_change();
}

void Theme::assign_colors(ImGuiStyle const& im_gui_style, ImPlotStyle const& im_plot_style)
{
    assign(_text_color, im_gui_style.Colors[ImGuiCol_Text]);
    assign(_text_disabled_color, im_gui_style.Colors[ImGuiCol_TextDisabled]);
    assign(_window_background_color, im_gui_style.Colors[ImGuiCol_WindowBg]);
    assign(_child_background_color, im_gui_style.Colors[ImGuiCol_ChildBg]);
    assign(_popup_background_color, im_gui_style.Colors[ImGuiCol_PopupBg]);
    assign(_border_color, im_gui_style.Colors[ImGuiCol_Border]);
    assign(_border_shadow_color, im_gui_style.Colors[ImGuiCol_BorderShadow]);
    assign(_frame_background_color, im_gui_style.Colors[ImGuiCol_FrameBg]);
    assign(_frame_background_hovered_color, im_gui_style.Colors[ImGuiCol_FrameBgHovered]);
    assign(_frame_background_active_color, im_gui_style.Colors[ImGuiCol_FrameBgActive]);
    assign(_title_background_color, im_gui_style.Colors[ImGuiCol_TitleBg]);
    assign(_title_background_active_color, im_gui_style.Colors[ImGuiCol_TitleBgActive]);
    assign(_title_background_collapsed_color, im_gui_style.Colors[ImGuiCol_TitleBgCollapsed]);
    assign(_menu_bar_background_color, im_gui_style.Colors[ImGuiCol_MenuBarBg]);
    assign(_scrollbar_background_color, im_gui_style.Colors[ImGuiCol_ScrollbarBg]);
    assign(_scrollbar_grab_color, im_gui_style.Colors[ImGuiCol_ScrollbarGrab]);
    assign(_scrollbar_grab_hovered_color, im_gui_style.Colors[ImGuiCol_ScrollbarGrabHovered]);
    assign(_scrollbar_grab_active_color, im_gui_style.Colors[ImGuiCol_ScrollbarGrabActive]);
    assign(_check_mark_color, im_gui_style.Colors[ImGuiCol_CheckMark]);
    assign(_slider_grab_color, im_gui_style.Colors[ImGuiCol_SliderGrab]);
    assign(_slider_grab_active_color, im_gui_style.Colors[ImGuiCol_SliderGrabActive]);
    assign(_button_color, im_gui_style.Colors[ImGuiCol_Button]);
    assign(_button_hovered_color, im_gui_style.Colors[ImGuiCol_ButtonHovered]);
    assign(_button_active_color, im_gui_style.Colors[ImGuiCol_ButtonActive]);
    assign(_header_color, im_gui_style.Colors[ImGuiCol_Header]);
    assign(_header_hovered_color, im_gui_style.Colors[ImGuiCol_HeaderHovered]);
    assign(_header_active_color, im_gui_style.Colors[ImGuiCol_HeaderActive]);
    assign(_separator_color, im_gui_style.Colors[ImGuiCol_Separator]);
    assign(_separator_hovered_color, im_gui_style.Colors[ImGuiCol_SeparatorHovered]);
    assign(_separator_active_color, im_gui_style.Colors[ImGuiCol_SeparatorActive]);
    assign(_resize_grip_color, im_gui_style.Colors[ImGuiCol_ResizeGrip]);
    assign(_resize_grip_hovered_color, im_gui_style.Colors[ImGuiCol_ResizeGripHovered]);
    assign(_resize_grip_active_color, im_gui_style.Colors[ImGuiCol_ResizeGripActive]);
    assign(_tab_color, im_gui_style.Colors[ImGuiCol_Tab]);
    assign(_tab_hovered_color, im_gui_style.Colors[ImGuiCol_TabHovered]);
    assign(_tab_active_color, im_gui_style.Colors[ImGuiCol_TabActive]);
    assign(_tab_unfocused_color, im_gui_style.Colors[ImGuiCol_TabUnfocused]);
    assign(_tab_unfocused_active_color, im_gui_style.Colors[ImGuiCol_TabUnfocusedActive]);
    assign(_table_header_background_color, im_gui_style.Colors[ImGuiCol_TableHeaderBg]);
    assign(_table_border_strong_color, im_gui_style.Colors[ImGuiCol_TableBorderStrong]);
    assign(_table_border_light_color, im_gui_style.Colors[ImGuiCol_TableBorderLight]);
    assign(_table_row_background_color, im_gui_style.Colors[ImGuiCol_TableRowBg]);
    assign(_table_row_background_alt_color, im_gui_style.Colors[ImGuiCol_TableRowBgAlt]);
    assign(_text_selected_background_color, im_gui_style.Colors[ImGuiCol_TextSelectedBg]);
    assign(_drag_drop_target_color, im_gui_style.Colors[ImGuiCol_DragDropTarget]);
    assign(_nav_highlight_color, im_gui_style.Colors[ImGuiCol_NavHighlight]);
    assign(_nav_windowing_highlight_color, im_gui_style.Colors[ImGuiCol_NavWindowingHighlight]);
    assign(_nav_windowing_dim_background_color, im_gui_style.Colors[ImGuiCol_NavWindowingDimBg]);
    assign(_modal_window_dim_background_color, im_gui_style.Colors[ImGuiCol_ModalWindowDimBg]);
    // implot
    assign(_plot_line_color, im_plot_style.Colors[ImPlotCol_Line]);
    assign(_plot_fill_color, im_plot_style.Colors[ImPlotCol_Fill]);
    assign(_plot_marker_outline_color, im_plot_style.Colors[ImPlotCol_MarkerOutline]);
    assign(_plot_marker_fill_color, im_plot_style.Colors[ImPlotCol_MarkerFill]);
    assign(_plot_error_bar_color, im_plot_style.Colors[ImPlotCol_ErrorBar]);
    assign(_plot_frame_background_color, im_plot_style.Colors[ImPlotCol_FrameBg]);
    assign(_plot_background_color, im_plot_style.Colors[ImPlotCol_PlotBg]);
    assign(_plot_border_color, im_plot_style.Colors[ImPlotCol_PlotBorder]);
    assign(_plot_legend_background_color, im_plot_style.Colors[ImPlotCol_LegendBg]);
    assign(_plot_legend_border_color, im_plot_style.Colors[ImPlotCol_LegendBorder]);
    assign(_plot_legend_text_color, im_plot_style.Colors[ImPlotCol_LegendText]);
    assign(_plot_title_text_color, im_plot_style.Colors[ImPlotCol_TitleText]);
    assign(_plot_inlay_text_color, im_plot_style.Colors[ImPlotCol_InlayText]);
    assign(_plot_axis_color, im_plot_style.Colors[ImPlotCol_AxisText]);
    assign(_plot_axis_grid_color, im_plot_style.Colors[ImPlotCol_AxisGrid]);
    assign(_plot_selection_color, im_plot_style.Colors[ImPlotCol_Selection]);
    assign(_plot_crosshairs_color, im_plot_style.Colors[ImPlotCol_Crosshairs]);
}

void Theme::make_light()
{
    ImGuiStyle im_gui_style;
    ImPlotStyle im_plot_style;
    ImGui::StyleColorsLight(&im_gui_style);
    ImPlot::StyleColorsLight(&im_plot_style);
    assign(im_plot_style.Colors[ImPlotCol_PlotBg], Color::named::white);
    assign(im_plot_style.Colors[ImPlotCol_FrameBg], Color(0xF5F5F5FF));
    im_plot_style.Colors[ImPlotCol_AxisGrid] = im_gui_style.Colors[ImGuiCol_Border];
    im_plot_style.Colors[ImPlotCol_PlotBorder] = im_gui_style.Colors[ImGuiCol_Border];
    assign_colors(im_gui_style, im_plot_style);
    _on_change();
}

void Theme::make_dark()
{
    ImGuiStyle im_gui_style;
    ImPlotStyle im_plot_style;
    ImGui::StyleColorsDark(&im_gui_style);
    ImPlot::StyleColorsDark(&im_plot_style);
    assign_colors(im_gui_style, im_plot_style);
    _on_change();
}

void Theme::make_classic()
{
    ImGuiStyle im_gui_style;
    ImPlotStyle im_plot_style;
    ImPlot::StyleColorsAuto(&im_plot_style);
    ImGui::StyleColorsClassic(&im_gui_style);
    ImPlot::StyleColorsClassic(&im_plot_style);
    assign_colors(im_gui_style, im_plot_style);
    _on_change();
}

std::unique_ptr<Theme> Theme::make_default()
{
    auto theme = std::make_unique<Theme>();
    theme->make_classic();
    return theme;
}

void Theme::_on_change()
{
    for (auto observer : _observer)
        observer->on_theme_changed();
}

}
