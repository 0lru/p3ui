#pragma once

#include <p3/Synchronizable.h>

struct GLFWvidmode;
struct GLFWmonitor;

namespace p3
{

    class Window;

    class VideoMode
        : public Synchronizable
    {
    public:
        VideoMode() = default;
        VideoMode(VideoMode const&) = default;
        VideoMode(GLFWmonitor*, int width, int height, int hz);
        int width() const;
        int height() const;
        int hz() const;

        GLFWmonitor* glfw_monitor() const;

    private:
        std::weak_ptr<Window> _window;
        GLFWmonitor* _monitor = nullptr;
        int _width;
        int _height;
        int _hz;
    };

}
