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

#include "Window.h"
#include "UserInterface.h"
#include "log.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>

#include <thread>
#include <implot.h>
#include <imgui_internal.h>

namespace
{

    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

}

namespace p3
{

    VideoMode::VideoMode(GLFWmonitor* monitor, int width, int height, int hz)
        : _monitor(monitor)
        , _width(width)
        , _height(height)
        , _hz(hz)
    {
    }

    int VideoMode::width() const
    {
        return _width;
    }

    int VideoMode::height() const
    {
        return _height;
    }

    int VideoMode::hz() const
    {
        return _hz;
    }

    GLFWmonitor * VideoMode::glfw_monitor() const
    {
        return _monitor;
    }

    Monitor::Monitor(GLFWmonitor* handle)
        : _handle(handle)
    {
    }

    VideoMode Monitor::mode() const
    {
        auto mode = glfwGetVideoMode(_handle);
        return VideoMode(_handle, mode->width, mode->height, mode->refreshRate);
    }

    std::vector<VideoMode> Monitor::modes() const
    {
        int mode_count;
        auto glfw_modes = glfwGetVideoModes(_handle, &mode_count);
        std::vector<VideoMode> modes(mode_count);
        for (int i = 0; i < mode_count; ++i)
            modes[i] = VideoMode(_handle, glfw_modes[i].width, glfw_modes[i].height, glfw_modes[i].refreshRate);
        return modes;
    }

    Window::Window(std::string title, std::size_t width, std::size_t height)
    {
        if (!glfwInit())
            throw std::runtime_error("could not init glfw");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwSetErrorCallback(error_callback);
        _glfw_window = std::shared_ptr<GLFWwindow>(
            glfwCreateWindow(int(width), int(height), title.c_str(), nullptr/*glfwGetPrimaryMonitor()*/, nullptr),
            glfwDestroyWindow);
        if (!_glfw_window)
        {
            glfwTerminate();
            throw std::runtime_error("failed to create glfw window");
        }

        glfwMakeContextCurrent(_glfw_window.get());
        // glfwSwapInterval(1); // Enable vsync
        gladLoadGL(glfwGetProcAddress);
    }

    Window::~Window()
    {
        _glfw_window.reset();
        glfwTerminate();
    }

    void Window::set_title(std::string title)
    {
        glfwSetWindowTitle(_glfw_window.get(), title.c_str());
        _title = std::move(title);
    }

    const std::string& Window::title() const
    {
        return _title;
    }

    void Window::set_user_interface(std::shared_ptr<UserInterface> user_interface)
    {
        _user_interface = std::move(user_interface);
        ImGui::SetCurrentContext(&_user_interface->im_gui_context());
        ImPlot::SetCurrentContext(&_user_interface->im_plot_context());
        ImGui_ImplGlfw_InitForOpenGL(_glfw_window.get(), true);
        ImGui_ImplOpenGL2_Init();
    }

    std::shared_ptr<UserInterface> const& Window::user_interface() const
    {
        return _user_interface;
    }

    bool Window::closed() const
    {
        return glfwWindowShouldClose(_glfw_window.get());
    }

    Window::Size Window::framebuffer_size() const
    {
        Size size;
        glfwGetFramebufferSize(_glfw_window.get(), &size.width, &size.height);
        return size;
    }

    void Window::frame()
    {
        auto framebuffer_size = this->framebuffer_size();
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        if (_user_interface)
            _user_interface->render(float(framebuffer_size.width), float(framebuffer_size.height));
        glViewport(0, 0, framebuffer_size.width, framebuffer_size.height);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(_glfw_window.get());
        glfwPollEvents();

        if (_timer.ticks() > std::chrono::milliseconds(1000))
        {
            std::cout << "fps: " << ImGui::GetIO().Framerate << std::endl;
            _timer.reset();
        }
        if (_target_framerate)
        {
            auto ms = _throttle_timer.ticks().count();
            auto ms_minimum = static_cast<decltype(ms)>(1000000000.0 / _target_framerate.value());
            if (ms < ms_minimum)
                std::this_thread::sleep_for(std::chrono::nanoseconds(ms_minimum - ms));
            _throttle_timer.reset();
        }
    }

    void Window::loop(UpdateCallback update_callback)
    {
        while (!closed())
        {
            if (update_callback)
                update_callback(std::static_pointer_cast<Window>(shared_from_this()));
            frame();
        }
    }

    void Window::set_target_framerate(std::optional<double> target_framerate)
    {
        _target_framerate = std::move(target_framerate);
        _throttle_timer.reset();
    }

    std::optional<double> const& Window::target_framerate() const
    {
        return _target_framerate;
    }

    std::optional<VideoMode> Window::video_mode() const
    {
        auto monitor = glfwGetWindowMonitor(_glfw_window.get());
        if (monitor)
        {
            auto mode = glfwGetVideoMode(monitor);
            return VideoMode(monitor, mode->width, mode->height, mode->refreshRate);
        }
        return std::nullopt;
    }

    void Window::set_video_mode(std::optional<VideoMode> mode)
    {
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
        else if(monitor)
        {
            glfwSetWindowMonitor(
                _glfw_window.get(), 
                nullptr, 
                _position.x, _position.y, _size.width, _size.height, 0);
        }
    }

    Window::Position Window::position() const
    {
        if (glfwGetWindowMonitor(_glfw_window.get()))
            glfwGetWindowPos(_glfw_window.get(), &_position.x, &_position.y);
        return _position;
    }

    void Window::set_position(Position position)
    {
        if (glfwGetWindowMonitor(_glfw_window.get()))
            _position = std::move(position);
        else
            glfwSetWindowPos(_glfw_window.get(), position.x, position.y);
    }

    Window::Size Window::size() const
    {
        if (glfwGetWindowMonitor(_glfw_window.get()))
            glfwGetWindowSize(_glfw_window.get(), &_size.width, &_size.height);
        return _size;
    }

    void Window::set_size(Size size)
    {
        if (glfwGetWindowMonitor(_glfw_window.get()))
            _size = std::move(size);
        else
            glfwSetWindowSize(_glfw_window.get(), size.width, size.height);
    }

    Monitor Window::primary_monitor()
    {
        return Monitor(glfwGetPrimaryMonitor());
    }

    std::vector<Monitor> Window::monitors()
    {
        int monitor_count;
        GLFWmonitor** glfw_monitors = glfwGetMonitors(&monitor_count);
        std::vector<Monitor> monitors(monitor_count);
        for (int i = 0; i < monitor_count; ++i)
            monitors[i] = Monitor(glfw_monitors[i]);
        return monitors;
    }

}
