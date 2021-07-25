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

#include "StyleTypes.h"

#include <memory>
#include <array>
#include <optional>
#include <string>
#include <functional>

namespace p3
{

    struct Theme;
    class UserInterface;
    class RenderBackend;

    class Context : public std::enable_shared_from_this<Context>
    {
    public:
        using MouseMove = std::optional<std::array<float, 2>>;
        using Postponed = std::function<void()>;

        Context(UserInterface&, RenderBackend&, MouseMove);
        ~Context();
        static Context& current();

        UserInterface& user_interface() const;
        RenderBackend& render_backend() const;

        float rem() const;
        Theme& theme() const;
        
        MouseMove const& mouse_move() const;
        float to_actual(Length const&) const;

        void postpone(Postponed);

    private:
        UserInterface& _user_interface;
        RenderBackend& _render_backend;
        MouseMove _mouse_move;
        std::vector<Postponed> _postponed;
    };

}
