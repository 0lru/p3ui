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

#include "Context.h"
#include "Font.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <stdexcept>
#include <iostream>

namespace p3
{

    namespace
    {
        thread_local Context* current_context = nullptr;
    }

    Context::Context()
        : _gui_context(ImGui::CreateContext(), &ImGui::DestroyContext)
        , _plot_context(ImPlot::CreateContext(), &ImPlot::DestroyContext)
    {
        make_current();
        ImGui::StyleColorsLight();
        {
            auto& style = ImGui::GetStyle();
            style.FrameRounding = 4;
            style.FramePadding = ImVec2(8, 8);
            style.FrameBorderSize = 1;
            style.WindowPadding = ImVec2(12, 12);
            style.WindowBorderSize = 0;
            style.WindowRounding = 0;
            style.ItemSpacing = { 8, 8 };
            style.ChildRounding = 4;
        }
        {
            auto& style = ImPlot::GetStyle();
            ImVec4* colors = style.Colors;
            colors[ImPlotCol_PlotBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            style.LineWeight = 3.0f;
        }

    }

    Context::~Context()
    {
        _plot_context.reset();
        _gui_context.reset();
    }

    Context::FontAtlas Context::font_atlas()
    {
        return FontAtlas(shared_from_this(), _gui_context->IO.Fonts);
    }

    Context::Font Context::load_font(std::string const& filename, float size)
    {
        auto font = _gui_context->IO.Fonts->AddFontFromFileTTF(filename.c_str(), size);
        font->FontSize = size;
        return Font(shared_from_this(), font);
    }

    void Context::set_default_font(Font font)
    {
        assert(font._context == shared_from_this());
        _rem = font.size();
        _gui_context->IO.FontDefault = font._font;
    }

    void Context::merge_font(std::string const& filename, float size)
    {
#define ICON_MIN_MDI 0xe000
#define ICON_MAX_MDI 0xeb4c
        ImFontConfig config;
        config.MergeMode = true;
        config.PixelSnapH = true;
        config.GlyphOffset.y = size / 4.0f;
        // config.OversampleH = 4;
        // config.OversampleV = 4;
        static const ImWchar icon_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
        _gui_context->IO.Fonts->AddFontFromFileTTF(filename.c_str(), size, &config, icon_ranges);
    }

    Context::Font Context::default_font()
    {
        auto& io = _gui_context->IO;
        return Font(shared_from_this(), io.FontDefault ? io.FontDefault : io.Fonts[0].Fonts[0]);
    }

    std::array<float, 2> Context::mouse_position() const
    {
        auto pos = ImGui::GetMousePos();
        return {pos.x, pos.y};
    }

    void Context::set_mouse_cursor_scale(float value)
    {
        ImGui::GetStyle().MouseCursorScale = value;
    }

    float Context::mouse_cursor_scale() const
    {
        return ImGui::GetStyle().MouseCursorScale;
    }

    void Context::set_anti_aliased_lines(bool value)
    {
        ImGui::GetStyle().AntiAliasedLines = value;
    }

    bool Context::anti_aliased_lines() const
    {
        return ImGui::GetStyle().AntiAliasedLines;
    }

    void Context::set_anti_aliased_fill(bool value)
    {
        ImGui::GetStyle().AntiAliasedFill = value;
    }

    bool Context::anti_aliased_fill() const
    {
        return ImGui::GetStyle().AntiAliasedFill;
    }

    void Context::set_curve_tessellation_tolerance(float value)
    {
        ImGui::GetStyle().CurveTessellationTol = value;
    }

    float Context::curve_tessellation_tolerance() const
    {
        return ImGui::GetStyle().CurveTessellationTol;
    }

    void Context::set_circle_tessellation_maximum_error(float value)
    {
        ImGui::GetStyle().CircleTessellationMaxError = value;
    }
    
    float Context::circle_tessellation_maximum_error() const
    {
        return ImGui::GetStyle().CircleTessellationMaxError;
    }

    void Context::make_current()
    {
        p3::current_context = this;
        ImGui::SetCurrentContext(_gui_context.get());
        ImPlot::SetCurrentContext(_plot_context.get());
    }

    Context& Context::current()
    {
        if (!p3::current_context)
            throw std::runtime_error("no context active");
        return *p3::current_context;
    }

    float Context::to_actual(Length const& length) const
    {
        if (std::holds_alternative<Pixels>(length))
            return std::get<Pixels>(length).value;
        if (std::holds_alternative<Em>(length))
            return _rem * std::get<Em>(length).value;
        return _rem * std::get<Rem>(length).value;
    }

    void Context::postpone(Postponed postponed)
    {
        _postponed.push_back(std::move(postponed));
    }

    void Context::execute_postponed()
    {
        for (auto& postponed : _postponed)
            postponed();
        _postponed.clear();
    }

    void Context::set_theme(Theme* theme)
    {
        _theme = theme;
    }

    Theme& Context::theme() const
    {
        return *_theme;
    }

}
