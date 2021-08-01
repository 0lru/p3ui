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
#define NOMINMAX
#include "Window.h"

#define GLFW_INCLUDE_NONE
#pragma warning( push )
#pragma warning( disable: 4005 )
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#pragma warning( pop )

#include "WindowTaskQueue.h"

#include <p3/UserInterface.h>
#include <p3/log.h>
#include <p3/backend/OpenGL3RenderBackend.h>
#include <backends/imgui_impl_glfw.h>

#include <thread>
#include <future>
#include <implot.h>
#include <imgui_internal.h>
#include <mutex>

namespace p3
{

    Window::Window(std::string title, std::size_t width, std::size_t height)
        : Node("MainWindow")
        , _render_backend(std::make_shared<OpenGL3RenderBackend>())
        , _task_queue(std::make_shared<TaskQueue>(this))
        , _thread()
    {
        auto promise = std::make_shared<std::promise<void>>();
        auto f = promise->get_future();
        _thread = std::thread([this, promise, title, width, height]() { thread(promise, title, width, height); });
        f.get();
        log_info("window created");
    }

    Window::~Window()
    {
        _thread.join();
        _glfw_window.reset();
        glfwTerminate();
    }

    void Window::thread(std::shared_ptr<std::promise<void>> promise, std::string title, std::size_t width, std::size_t height)
    {
        if (!glfwInit())
            log_fatal("could not init glfw");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwSetErrorCallback([](int code, char const* text) {
            log_fatal("glfw error, code={}, text=\"{}\"", code, text);
        });

        _glfw_window = std::shared_ptr<GLFWwindow>(
            glfwCreateWindow(int(width), int(height), title.c_str(), nullptr, nullptr),
            glfwDestroyWindow);
        if (!_glfw_window)
        {
            glfwTerminate();
            promise->set_exception(std::make_exception_ptr(std::runtime_error("failed to create glfw window")));
            return;
        }
        glfwSetWindowUserPointer(_glfw_window.get(), this);
        glfwMakeContextCurrent(_glfw_window.get());
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(_vsync ? 1 : 0);
        log_debug("window created, vsync={}", _vsync);
        promise->set_value();
        try
        {
            while (!glfwWindowShouldClose(_glfw_window.get()))
            {
                glfwWaitEventsTimeout(0.5);
                _task_queue->process();
                frame();
            }
            //
            // render task could be pending. need
            // to empty the queue, since it may blocks
            // the ui thread
            _task_queue->process();
            if (_user_interface)
                _serve_promise.set_value();
        }
        catch (...)
        {
            if (_user_interface)
                _serve_promise.set_exception(std::current_exception());
        }
    }

    void Window::set_title(std::string title)
    {
        return _task_queue->run_blocking_task([this, title] {
            glfwSetWindowTitle(_glfw_window.get(), title.c_str());
            _title = std::move(title);
        });
    }

    std::string Window::title() const
    {
        return _task_queue->run_blocking_task([this] {
            return _title;
        });
    }

    void Window::serve(
        Promise<void>&& promise,
        std::shared_ptr<UserInterface> user_interface,
        std::shared_ptr<p3::TaskQueue> queue)
    {
        _task_queue->run_blocking_task([this,
            user_interface,
            promise{ std::move(promise) },
            queue{ std::move(queue) }
        ]() mutable {
            _user_interface = std::move(user_interface);

            _user_interface->synchronize_with(*this);
            _user_interface->set_parent(this);
            _serve_promise = std::move(promise);
            _serve_queue = std::move(queue);

            //
            // this is global a. t. m. ...
            ImGui::SetCurrentContext(&_user_interface->im_gui_context());
            ImPlot::SetCurrentContext(&_user_interface->im_plot_context());

            //
            // reset callback state
            glfwSetMouseButtonCallback(_glfw_window.get(), GlfwMouseButtonCallback);
            glfwSetScrollCallback(_glfw_window.get(), GlfwScrollCallback);
            glfwSetKeyCallback(_glfw_window.get(), GlfwKeyCallback);
            glfwSetCharCallback(_glfw_window.get(), GlfwCharCallback);
            glfwSetFramebufferSizeCallback(_glfw_window.get(), GlfwFramebufferSizeCallback);
            // glfwSetWindowSizeCallback
            // glfwSetCursorPosCallback(_glfw_window.get(), GlfwCursorPosCallback);

            //
            // init member variables
            glfwGetCursorPos(_glfw_window.get(), &_window_state.mouse[0], &_window_state.mouse[1]);
            glfwGetFramebufferSize(_glfw_window.get(), &_window_state.framebuffer_size.width, &_window_state.framebuffer_size.height);

            //
            // let imggui "hook into"
            _render_backend->init();
            ImGui_ImplGlfw_InitForOpenGL(_glfw_window.get(), true);
        });
    }

    void Window::frame()
    {
        MousePosition mouse_position;
        glfwGetCursorPos(_glfw_window.get(), &(mouse_position[0]), &mouse_position[1]);
        Context::MouseMove mouse_move = std::nullopt;
        if (_window_state.mouse[0] != mouse_position[0] ||
            _window_state.mouse[1] != mouse_position[1])
        {
            mouse_move = std::array<float, 2>{
                float(mouse_position[0] - _window_state.mouse[0]),
                    float(mouse_position[1] - _window_state.mouse[1])
            };
            std::swap(_window_state.mouse, mouse_position);
        }
        if (mouse_move)
            _idle_timer.reset();

        if (_idle_timeout)
        {
            if (_idle_timer.time() > _idle_timeout.value() &&
                _frame_timer.time() < _idle_frame_time)
            {
                return;
            }
        }
        _frame_timer.reset();
        if (_user_interface)
        {
            _render_backend->new_frame();
            ImGui_ImplGlfw_NewFrame();
            {
                {
                    auto guard = _user_interface->lock();
                    Context context(*_user_interface, *_serve_queue, *_render_backend, mouse_move);
                    _user_interface->render(context, float(_window_state.framebuffer_size.width), float(_window_state.framebuffer_size.height));
                }
                glViewport(0, 0, _window_state.framebuffer_size.width, _window_state.framebuffer_size.height);
                if (_user_interface)
                    _render_backend->render(*_user_interface);
            }
            glFlush();
            glfwSwapBuffers(_glfw_window.get());
        }
    }

    std::shared_ptr<UserInterface> Window::user_interface() const
    {
        return _task_queue->run_blocking_task([this] {
            return _user_interface;
        });
    }

    bool Window::closed() const
    {
        return _task_queue->run_blocking_task([this] {
            return glfwWindowShouldClose(_glfw_window.get());
        });
    }

    Window::Size Window::framebuffer_size() const
    {
        return _task_queue->run_blocking_task([this] {
            Size size;
            glfwGetFramebufferSize(_glfw_window.get(), &size.width, &size.height);
            return size;
        });
    }

    double Window::frames_per_second() const
    {
        return _task_queue->run_blocking_task([this] {
            return ImGui::GetIO().Framerate;
        });
    }

    double Window::time_till_enter_idle_mode() const
    {
        return _task_queue->run_blocking_task([this] {
            return _idle_timeout
                ? std::max(0., std::chrono::duration<double>(_idle_timeout.value() - _idle_timer.time()).count())
                : 0.;
        });
    }

    std::optional<VideoMode> Window::video_mode() const
    {
        return _task_queue->run_blocking_task([this]()-> std::optional<VideoMode> {
            auto monitor = glfwGetWindowMonitor(_glfw_window.get());
            if (monitor)
            {
                auto mode = glfwGetVideoMode(monitor);
                return VideoMode(monitor, mode->width, mode->height, mode->refreshRate);
            }
            return std::nullopt;
        });
    }

    void Window::set_video_mode(std::optional<VideoMode> mode)
    {
        return _task_queue->run_blocking_task([this, mode] {
            auto monitor = glfwGetWindowMonitor(_glfw_window.get());
            if (mode)
            {
                if (!monitor)
                {
                    glfwGetWindowPos(_glfw_window.get(), &_position.x, &_position.y);
                    glfwGetWindowSize(_glfw_window.get(), &_size.width, &_size.height);
                }
                glfwSetWindowMonitor(
                    _glfw_window.get(),
                    mode.value().glfw_monitor(),
                    0, 0,
                    mode.value().width(), mode.value().height(),
                    mode.value().hz());
            }
            else if (monitor)
            {
                glfwSetWindowMonitor(
                    _glfw_window.get(),
                    nullptr,
                    _position.x, _position.y, _size.width, _size.height, 0);
            }
            glfwSwapInterval(_vsync ? 1 : 0);
        });
    }

    Window::Position Window::position() const
    {
        return _task_queue->run_blocking_task([this] {
            if (!glfwGetWindowMonitor(_glfw_window.get()))
                glfwGetWindowPos(_glfw_window.get(), &_position.x, &_position.y);
            return _position;
        });
    }

    void Window::set_position(Position position)
    {
        return _task_queue->run_blocking_task([this, position]() {
            if (glfwGetWindowMonitor(_glfw_window.get()))
                _position = std::move(position);
            else
                glfwSetWindowPos(_glfw_window.get(), position.x, position.y);
        });
    }

    Window::Size Window::size() const
    {
        return _task_queue->run_blocking_task([this] {
            if (!glfwGetWindowMonitor(_glfw_window.get()))
                glfwGetWindowSize(_glfw_window.get(), &_size.width, &_size.height);
            return _size;
        });
    }

    void Window::set_size(Size size)
    {
        return _task_queue->run_blocking_task([this, size]() {
            if (glfwGetWindowMonitor(_glfw_window.get()))
                _size = std::move(size);
            else
                glfwSetWindowSize(_glfw_window.get(), size.width, size.height);
        });
    }

    Monitor Window::primary_monitor() const
    {
        return _task_queue->run_blocking_task([this] {
            std::shared_ptr<Node const> x = shared_from_this();
            return Monitor(std::const_pointer_cast<Window>(shared_ptr()), glfwGetPrimaryMonitor());
        });
    }

    std::vector<Monitor> Window::monitors() const
    {
        return _task_queue->run_blocking_task([this] {
            int monitor_count;
            GLFWmonitor** glfw_monitors = glfwGetMonitors(&monitor_count);
            std::vector<Monitor> monitors(monitor_count);
            for (int i = 0; i < monitor_count; ++i)
                monitors[i] = Monitor(std::const_pointer_cast<Window>(shared_ptr()), glfw_monitors[i]);
            return monitors;
        });
    }

    void Window::set_vsync(bool vsync)
    {
        return _task_queue->run_blocking_task([this, vsync] {
            _vsync = vsync;
            glfwSwapInterval(_vsync ? 1 : 0);
        });
    }

    bool Window::vsync() const
    {
        return _task_queue->run_blocking_task([this] {
            return _vsync;
        });
    }

    void Window::GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->_idle_timer.reset();
    }

    void Window::GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->_idle_timer.reset();
    }

    void Window::GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->_idle_timer.reset();
    }

    void Window::GlfwCharCallback(GLFWwindow* window, unsigned int c)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->_idle_timer.reset();
    }

    void Window::GlfwFramebufferSizeCallback(GLFWwindow* window, int w, int h)
    {
        auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->_window_state.framebuffer_size.width = w;
        self->_window_state.framebuffer_size.height = h;
        self->_idle_timer.reset();
    }

    void Window::GlfwCursorPosCallback(GLFWwindow* window, double x, double y)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->_idle_timer.reset();
    }

    void Window::set_idle_timeout(std::optional<Seconds> idle_timeout)
    {
        _idle_timeout = idle_timeout;
    }

    std::optional<Window::Seconds> Window::idle_timeout() const
    {
        return _idle_timeout;
    }

    void Window::set_idle_frame_time(Seconds idle_frame_time)
    {
        _idle_frame_time = idle_frame_time;
    }

    Window::Seconds Window::idle_frame_time() const
    {
        return _idle_frame_time;
    }

    Monitor Window::monitor() const
    {
        auto handle = glfwGetWindowMonitor(_glfw_window.get());
        if (handle == nullptr)
            handle = glfwGetPrimaryMonitor();
        return Monitor(std::const_pointer_cast<Window>(shared_ptr()), handle);
    }

    std::shared_ptr<Window const> Window::shared_ptr() const
    {
        return std::static_pointer_cast<Window const>(shared_from_this());
    }

    bool Monitor::operator==(Monitor const& other) const
    {
        auto w1 = _window.lock();
        auto w2 = other._window.lock();
        return w1 == w2 && _handle == other._handle;
    }

    double Monitor::dpi() const
    {
        auto window = _window.lock();
        if (!window)
            return 96.0;
        int width, height;
        const GLFWvidmode* mode = glfwGetVideoMode(_handle);
        glfwGetMonitorPhysicalSize(_handle, &width, &height);
        // 25,4 = one inch in [mm]
        return width == 0
            ? 96.0
            : mode->width / (width / 25.4);
    }

    void Window::redraw()
    {
        // possibly wakes up the thread (force state evaluation)
        glfwPostEmptyEvent();
    }

}
