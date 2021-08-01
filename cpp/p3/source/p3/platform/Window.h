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

#include "Monitor.h"
#include "Timer.h"

#include <p3/Context.h>
#include <p3/Node.h>
#include <p3/Theme.h>
#include <p3/TaskQueue.h>

#include <imgui.h>

#include <memory>
#include <string>
#include <functional>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <optional>
#include <atomic>

struct GLFWwindow;

namespace p3
{

    class ChildWindow;
    class MenuBar;
    class Popup;
    class RenderBackend;

    class Window : public Node
    {
    public:
        class TaskQueue;

        using MousePosition = std::array<double, 2>;
        using UpdateCallback = std::function<void(std::shared_ptr<Window>)>;
        using Seconds = std::chrono::duration<double>;

        struct Position { int x; int y; };
        struct Size { int width; int height; };

        Window(std::string title, std::size_t width, std::size_t height);
        ~Window();

        void set_title(std::string);
        std::string title() const;

        void serve(Promise<void>&&, std::shared_ptr<UserInterface>, std::shared_ptr<p3::TaskQueue>);
        std::shared_ptr<UserInterface> user_interface() const;

        void frame();
        bool closed() const;

        std::optional<VideoMode> video_mode() const;
        void set_video_mode(std::optional<VideoMode>);

        Monitor monitor() const;
        Monitor primary_monitor() const;
        std::vector<Monitor> monitors() const;

        Position position() const;
        void set_position(Position);

        Size size() const;
        void set_size(Size);

        Size framebuffer_size() const;

        void set_vsync(bool);
        bool vsync() const;

        void set_idle_timeout(std::optional<Seconds>);
        std::optional<Seconds> idle_timeout() const;

        void set_idle_frame_time(Seconds);
        Seconds idle_frame_time() const;

        double frames_per_second() const;
        double time_till_enter_idle_mode() const;

        void redraw() override;

        std::shared_ptr<Window const> shared_ptr() const;

    private:
        bool _vsync = true;
        std::string _title;

        std::shared_ptr<RenderBackend> _render_backend;
        std::shared_ptr<TaskQueue> _task_queue;

        void thread(std::shared_ptr<std::promise<void>>, std::string title, std::size_t width, std::size_t height);
        std::thread _thread;

        using Task = std::function<void()>;

        struct
        {
            MousePosition mouse{ 0.f, 0.f };
            Size framebuffer_size;
        } _window_state;

        mutable Position _position{ 10, 10 };
        mutable Size _size{ 1024, 768 };

        std::shared_ptr<GLFWwindow> _glfw_window = nullptr;

        Promise<void> _serve_promise;
        std::shared_ptr<p3::TaskQueue> _serve_queue = nullptr;
        std::shared_ptr<UserInterface> _user_interface = nullptr;

        Timer _frame_timer;
        Timer _idle_timer;
        std::optional<Seconds> _idle_timeout = std::nullopt;
        Seconds _idle_frame_time = Seconds(1);

    private:
        static void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void GlfwCharCallback(GLFWwindow* window, unsigned int c);
        static void GlfwFramebufferSizeCallback(GLFWwindow* window, int, int);
        static void GlfwCursorPosCallback(GLFWwindow* window, double, double);
    };

}
