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

    void Window::frame()
    {
        glfwGetFramebufferSize(_glfw_window.get(), &_width, &_height);
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        if (_user_interface)
            _user_interface->render(float(_width), float(_height));
        glViewport(0, 0, _width, _height);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(_glfw_window.get());
        glfwPollEvents();

        if (_timer.ticks() > std::chrono::milliseconds(1000))
        {
            std::cout << "fps: " << ImGui::GetIO().Framerate << std::endl;
            _timer.reset();
        }
        auto ms = _throttle_timer.ticks().count();
        auto ms_minimum = static_cast<decltype(ms)>(1000000000.0 / target_frame_rate);
        if (ms < ms_minimum)
            std::this_thread::sleep_for(std::chrono::nanoseconds(ms_minimum - ms));
        _throttle_timer.reset();
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

}