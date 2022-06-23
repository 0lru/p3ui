#define NOMINMAX
#include "Window.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace p3 {
Monitor::Monitor(std::shared_ptr<Window> window, GLFWmonitor* handle)
    : _window(window)
    , _handle(handle)
{
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
