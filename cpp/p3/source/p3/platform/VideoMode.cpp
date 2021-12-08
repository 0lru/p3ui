#define NOMINMAX
#include "Window.h"
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

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

    GLFWmonitor* VideoMode::glfw_monitor() const
    {
        return _monitor;
    }

    bool VideoMode::operator==(const VideoMode& mode) const
    {
        return _monitor == mode._monitor
                && _width == mode._width
                && _height == mode._height
                && _hz == mode._hz;
    }

}
