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
#include "Node.h"
#include "Theme.h"

#include <imgui.h>

#include <memory>
#include <string>
#include <functional>
#include <string>
#include <chrono>

struct GLFWwindow;

namespace p3
{

    class ChildWindow;
    class MenuBar;
    class Popup;

    class Timer
    {
    public:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;

        std::chrono::milliseconds reset()
        {
            auto now = Clock::now();
            auto delta = now - _timepoint;
            _timepoint = now;
            return std::chrono::duration_cast<std::chrono::milliseconds>(delta);
        }

        std::chrono::nanoseconds ticks()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - _timepoint);
        }

    private:
        TimePoint _timepoint;
    };

    class Window : public std::enable_shared_from_this<Window>
    {
    public:
        using UpdateCallback = std::function<void(std::shared_ptr<Window>)>;

        Window(std::string title, std::size_t width, std::size_t height);
        ~Window();

        void set_title(std::string);
        const std::string& title() const;

        void set_user_interface(std::shared_ptr<UserInterface>);
        std::shared_ptr<UserInterface> const& user_interface() const;

        void frame();
        void loop(UpdateCallback);
        bool closed() const;

    private:
        std::string _title;
        int _width = 1024, _height = 768;
        std::shared_ptr<GLFWwindow> _glfw_window;

        std::shared_ptr<UserInterface> _user_interface;

        Timer _timer;
        float target_frame_rate = 60.f;
    };

}
