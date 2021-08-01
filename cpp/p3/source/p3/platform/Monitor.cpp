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
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace p3
{
    Monitor::Monitor(std::shared_ptr<Window> window, GLFWmonitor* handle)
        : _window(window)
        , _handle(handle)
    {
        synchronize_with(*window);
    }

    std::string Monitor::name() const
    {
        return glfwGetMonitorName(_handle);
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
}