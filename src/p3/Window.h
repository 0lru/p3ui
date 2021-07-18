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
#include <optional>

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

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

        std::chrono::nanoseconds reset()
        {
            auto now = Clock::now();
            auto delta = now - _timepoint;
            _timepoint = now;
            return std::chrono::duration_cast<std::chrono::nanoseconds>(delta);
        }

        std::chrono::nanoseconds ticks()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - _timepoint);
        }

    private:
        TimePoint _timepoint;
    };

    class VideoMode
    {
    public:
        VideoMode() = default;
        VideoMode(VideoMode const&) = default;
        VideoMode(GLFWmonitor *, int width, int height, int hz);
        int width() const;
        int height() const;
        int hz() const;

        GLFWmonitor* glfw_monitor() const;

    private:
        GLFWmonitor* _monitor = nullptr;
        int _width;
        int _height;
        int _hz;
    };

    class Monitor
    {
    public:
        Monitor() = default;
        Monitor(Monitor const&) = default;
        explicit Monitor(GLFWmonitor*);

        bool operator==(Monitor const&) const = default;
        bool operator!=(Monitor const&) const = default;

        VideoMode mode() const;
        void set_mode(VideoMode);

        std::vector<VideoMode> modes() const;

        std::string name() const;

    private:
        GLFWmonitor* _handle = nullptr;
    };

    class Window : public std::enable_shared_from_this<Window>
    {
    public:
        using UpdateCallback = std::function<void(std::shared_ptr<Window>)>;

        struct Position { int x; int y; };
        struct Size { int width; int height; };

        Window(std::string title, std::size_t width, std::size_t height);
        ~Window();

        void set_title(std::string);
        const std::string& title() const;

        void set_user_interface(std::shared_ptr<UserInterface>);
        std::shared_ptr<UserInterface> const& user_interface() const;

        void frame();
        void loop(UpdateCallback);
        bool closed() const;

        void set_target_framerate(std::optional<double>);
        std::optional<double> const& target_framerate() const;

        std::optional<VideoMode> video_mode() const;
        void set_video_mode(std::optional<VideoMode>);

        static Monitor primary_monitor();
        static std::vector<Monitor> monitors();

        Position position() const;
        void set_position(Position);

        Size size() const;
        void set_size(Size);
        
        Size framebuffer_size() const;

        void set_vsync(bool);
        bool vsync() const;

    private:
        bool _vsync = true;
        std::string _title;
        mutable Position _position{ 10, 10 };
        mutable Size _size{ 1024, 768 };
        std::shared_ptr<GLFWwindow> _glfw_window;

        std::shared_ptr<UserInterface> _user_interface;

        Timer _timer;
        Timer _throttle_timer;
        std::optional<double> _target_framerate = std::nullopt;
    };

}
