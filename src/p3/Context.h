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

#include <memory>
#include <array>
#include <optional>
#include <string>
#include <functional>

struct ImGuiContext;
struct ImPlotContext;

namespace p3
{

    struct Theme;

    class Context : public std::enable_shared_from_this<Context>
    {
    public:
        using Postponed = std::function<void()>;
        class Font;
        class FontAtlas;

        Context();
        ~Context();

        FontAtlas font_atlas();
        Font load_font(std::string const&, float size);
        Font default_font();
        void merge_font(std::string const&, float size);
        void set_default_font(Font);

        void set_mouse_cursor_scale(float);
        float mouse_cursor_scale() const;

        void set_anti_aliased_lines(bool);
        bool anti_aliased_lines() const;

        void set_anti_aliased_fill(bool);
        bool anti_aliased_fill() const;
        
        void set_curve_tessellation_tolerance(float);
        float curve_tessellation_tolerance() const;

        void set_circle_tessellation_maximum_error(float);
        float circle_tessellation_maximum_error() const;

        std::array<float, 2> mouse_position() const;

        void make_current();
        static Context& current();

        void set_rem(float rem) { _rem = rem; }
        float rem() const { return _rem; }

        void set_mouse_delta(std::optional<std::array<float, 2>>);
        std::optional<std::array<float, 2>> const& mouse_delta() const;

        float to_actual(Length const &) const;

        void postpone(Postponed);
        void execute_postponed();

        void set_theme(Theme*);
        Theme& theme() const;

    private:
        float _rem = 0.f;
        std::optional<std::array<float, 2>> _mouse_delta;
        std::shared_ptr<ImGuiContext> _gui_context;
        std::shared_ptr<ImPlotContext> _plot_context;
        std::vector<Postponed> _postponed;
        Theme *_theme;
    };

    inline void Context::set_mouse_delta(std::optional<std::array<float, 2>> mouse_delta)
    {
        _mouse_delta = std::move(mouse_delta);
    }

    inline std::optional<std::array<float, 2>> const& Context::mouse_delta() const
    {
        return _mouse_delta;
    }

}
