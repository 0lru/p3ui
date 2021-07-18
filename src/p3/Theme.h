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
#pragma once

#include "LengthPercentageTypes.h"
#include "Color.h"
#include "OnScopeExit.h"

#include <array>
#include <optional>

#include <memory>
#include <tuple>
#include <vector>

struct ImGuiStyle;

namespace p3
{

    class Context;
    using Float2 = std::array<float, 2>;

    struct Theme
    {
        class Observer
        {
        public:
            virtual ~Observer() = default;
            virtual void on_theme_changed() = 0;
        };

        void add_observer(Observer *);
        void remove_observer(Observer *);

        using ApplyFunction = std::function<OnScopeExit(void)>;
        ApplyFunction compile(Context const&);

        float alpha() const;
        void set_alpha(float);

        Length2 window_padding() const;
        void set_window_padding(Length2);

        Length window_rounding() const;
        void set_window_rounding(Length);

        Length window_border_size() const;
        void set_window_border_size(Length);

        Length2 window_min_size() const;
        void set_window_min_size(Length2);

        Float2 window_title_align() const;
        void set_window_title_align(Float2);

        Length child_rounding() const;
        void set_child_rounding(Length);

        Length child_border_size() const;
        void set_child_border_size(Length);

        Length popup_rounding() const;
        void set_popup_rounding(Length);

        Length popup_border_size() const;
        void set_popup_border_size(Length);

        Length2 frame_padding() const;
        void set_frame_padding(Length2);

        Length frame_rounding() const;
        void set_frame_rounding(Length);

        Length frame_border_size() const;
        void set_frame_border_size(Length);

        Length2 item_spacing() const;
        void set_item_spacing(Length2);

        Length2 item_inner_spacing() const;
        void set_item_inner_spacing(Length2);

        Length indent_spacing() const;
        void set_indent_spacing(Length);

        Length2 cell_padding() const;
        void set_cell_padding(Length2);

        Length scrollbar_size() const;
        void set_scrollbar_size(Length);

        Length scrollbar_rounding() const;
        void set_scrollbar_rounding(Length);

        Length grab_min_size() const;
        void set_grab_min_size(Length);

        Length grab_rounding() const;
        void set_grab_rounding(Length);

        Length tab_rounding() const;
        void set_tab_rounding(Length);

        Float2 button_text_align() const;
        void set_button_text_align(Float2);

        Float2 selectable_text_align() const;
        void set_selectable_text_align(Float2);

        Color text_color() const;
        void set_text_color(Color);

        Color text_disabled_color() const;
        void set_text_disabled_color(Color);

        Color window_background_color() const;
        void set_window_background_color(Color);

        Color child_background_color() const;
        void set_child_background_color(Color);

        Color popup_background_color() const;
        void set_popup_background_color(Color);

        Color border_color() const;
        void set_border_color(Color);

        Color border_shadow_color() const;
        void set_border_shadow_color(Color);

        Color frame_background_color() const;
        void set_frame_background_color(Color);

        Color frame_background_hovered_color() const;
        void set_frame_background_hovered_color(Color);

        Color frame_background_active_color() const;
        void set_frame_background_active_color(Color);

        Color title_background_color() const;
        void set_title_background_color(Color);

        Color title_background_active_color() const;
        void set_title_background_active_color(Color);

        Color title_background_collapsed_color() const;
        void set_title_background_collapsed_color(Color);

        Color menu_bar_background_color() const;
        void set_menu_bar_background_color(Color);

        Color scrollbar_background_color() const;
        void set_scrollbar_background_color(Color);

        Color scrollbar_grab_color() const;
        void set_scrollbar_grab_color(Color);

        Color scrollbar_grab_hovered_color() const;
        void set_scrollbar_grab_hovered_color(Color);

        Color scrollbar_grab_active_color() const;
        void set_scrollbar_grab_active_color(Color);

        Color check_mark_color() const;
        void set_check_mark_color(Color);

        Color slider_grab_color() const;
        void set_slider_grab_color(Color);

        Color slider_grab_active_color() const;
        void set_slider_grab_active_color(Color);

        Color button_color() const;
        void set_button_color(Color);

        Color button_hovered_color() const;
        void set_button_hovered_color(Color);

        Color button_active_color() const;
        void set_button_active_color(Color);

        Color header_color() const;
        void set_header_color(Color);

        Color header_hovered_color() const;
        void set_header_hovered_color(Color);

        Color header_active_color() const;
        void set_header_active_color(Color);

        Color separator_color() const;
        void set_separator_color(Color);

        Color separator_hovered_color() const;
        void set_separator_hovered_color(Color);

        Color separator_active_color() const;
        void set_separator_active_color(Color);

        Color resize_grip_color() const;
        void set_resize_grip_color(Color);

        Color resize_grip_hovered_color() const;
        void set_resize_grip_hovered_color(Color);

        Color resize_grip_active_color() const;
        void set_resize_grip_active_color(Color);

        Color tab_color() const;
        void set_tab_color(Color);

        Color tab_hovered_color() const;
        void set_tab_hovered_color(Color);

        Color tab_active_color() const;
        void set_tab_active_color(Color);

        Color tab_unfocused_color() const;
        void set_tab_unfocused_color(Color);

        Color tab_unfocused_active_color() const;
        void set_tab_unfocused_active_color(Color);

        Color table_header_background_color() const;
        void set_table_header_background_color(Color);

        Color table_border_strong_color() const;
        void set_table_border_strong_color(Color);

        Color table_border_light_color() const;
        void set_table_border_light_color(Color);

        Color table_row_background_color() const;
        void set_table_row_background_color(Color);

        Color table_row_background_alt_color() const;
        void set_table_row_background_alt_color(Color);

        Color text_selected_background_color() const;
        void set_text_selected_background_color(Color);

        Color drag_drop_target_color() const;
        void set_drag_drop_target_color(Color);

        Color nav_highlight_color() const;
        void set_nav_highlight_color(Color);

        Color nav_windowing_highlight_color() const;
        void set_nav_windowing_highlight_color(Color);

        Color nav_windowing_dim_background_color() const;
        void set_nav_windowing_dim_background_color(Color);

        Color modal_window_dim_background_color() const;
        void set_modal_window_dim_background_color(Color);

        void assign_colors(ImGuiStyle const&);
        void make_light();
        void make_dark();
        void make_classic();
        static std::unique_ptr<Theme> make_default();

    private:
        void _on_change();
        std::vector<Observer*> _observer;

        float _alpha = 1.f;
        Length2 _window_padding{ .35f | em, 0.35 | em };
        Length _window_rounding{ 2 | px };
        Length _window_border_size{ 1 | px };
        Length2 _window_min_size{ 10 | px, 10 | px };
        Float2 _window_title_align{ 0.f, 0.5f };
        Length _child_rounding{ 2 | px };
        Length _child_border_size{ 1 | px };
        Length _popup_rounding{ 2 | px };
        Length _popup_border_size{ 1 | px };
        Length2 _frame_padding{ .35f | em, .35f | em };
        Length _frame_rounding{ 2 | px };
        Length _frame_border_size{ 1 | px };
        Length2 _item_spacing{ .35f | em, .35f | em };
        Length2 _item_inner_spacing{ .35f | em, .35f | em };
        Length _indent_spacing{ 1 | em };
        Length2 _cell_padding{ .35f | em, .35f | em };
        Length _scrollbar_size{ 1 | em };
        Length _scrollbar_rounding{ 2 | px };
        Length _grab_min_size{ .5f | em };
        Length _grab_rounding{ 0 | px };
        Length _tab_rounding{ 2 | px };
        Float2 _button_text_align{ .5f, .5f };
        Float2 _selectable_text_align{ 0.f, 0.f };
        Color _text_color;
        Color _text_disabled_color;
        Color _window_background_color;
        Color _child_background_color;
        Color _popup_background_color;
        Color _border_color;
        Color _border_shadow_color;
        Color _frame_background_color;
        Color _frame_background_hovered_color;
        Color _frame_background_active_color;
        Color _title_background_color;
        Color _title_background_active_color;
        Color _title_background_collapsed_color;
        Color _menu_bar_background_color;
        Color _scrollbar_background_color;
        Color _scrollbar_grab_color;
        Color _scrollbar_grab_hovered_color;
        Color _scrollbar_grab_active_color;
        Color _check_mark_color;
        Color _slider_grab_color;
        Color _slider_grab_active_color;
        Color _button_color;
        Color _button_hovered_color;
        Color _button_active_color;
        Color _header_color;
        Color _header_hovered_color;
        Color _header_active_color;
        Color _separator_color;
        Color _separator_hovered_color;
        Color _separator_active_color;
        Color _resize_grip_color;
        Color _resize_grip_hovered_color;
        Color _resize_grip_active_color;
        Color _tab_color;
        Color _tab_hovered_color;
        Color _tab_active_color;
        Color _tab_unfocused_color;
        Color _tab_unfocused_active_color;
        Color _table_header_background_color;
        Color _table_border_strong_color;
        Color _table_border_light_color;
        Color _table_row_background_color;
        Color _table_row_background_alt_color;
        Color _text_selected_background_color;
        Color _drag_drop_target_color;
        Color _nav_highlight_color;
        Color _nav_windowing_highlight_color;
        Color _nav_windowing_dim_background_color;
        Color _modal_window_dim_background_color;
    };

    /*
        // style_set()->set<Style::ButtonColor>(Color(0x4296F966));
        // style_set()->set<Style::TextColor>(Color("black"));
        // style_set()->set<Style::BorderColor>(Color(0, 0, 0, 100));
        // style_set()->set<Style::TableRowBgColor>(Color(0xFFFFFFFF));
        // style_set()->set<Style::TableRowBgAltColor>(Color(0xeeeeeeee));
        // style_set()->set<Style::TableBorderLightColor>(Color(0x96969664));
        // style_set()->set<Style::TableBorderStrongColor>(Color(0, 0, 0, 100));
        // style_set()->set<Style::TableHeaderBgColor>(Color(0x63779166));
        // style_set()->set<Style::TabActiveColor>(Color(0x4296F966));
        // style_set()->set<Style::TabColor>(Color(0xf0f0f0ff));

        // ImPlot
        PlotLineColor,
        PlotFillColor,
        PlotMarkerOutlineColor,
        PlotMarkerFillColor,
        PlotErrorBarColor,
        PlotFrameBgColor,
        PlotPlotBgColor,
        PlotPlotBorderColor,
        PlotLegendBgColor,
        PlotLegendBorderColor,
        PlotLegendTextColor,
        PlotTitleTextColor,
        PlotInlayTextColor,
        PlotXAxisColor,
        PlotXAxisGridColor,
        PlotYAxisColor,
        PlotYAxisGridColor,
        PlotYAxis2Color,
        PlotYAxisGrid2Color,
        PlotYAxis3Color,
        PlotYAxisGrid3Color,
        PlotSelectionColor,
        PlotQueryColor,
        PlotCrosshairsColor
    };
    */
}
