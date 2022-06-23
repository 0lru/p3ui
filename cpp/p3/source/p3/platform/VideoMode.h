#pragma once

#include <memory>

struct GLFWvidmode;
struct GLFWmonitor;

namespace p3 {

class Window;

class VideoMode {
public:
    VideoMode() = default;
    VideoMode(VideoMode const&) = default;
    VideoMode(GLFWmonitor*, int width, int height, int hz);
    int width() const;
    int height() const;
    int hz() const;

    GLFWmonitor* glfw_monitor() const;
    bool operator==(const VideoMode&) const;

private:
    std::weak_ptr<Window> _window;
    GLFWmonitor* _monitor = nullptr;
    int _width;
    int _height;
    int _hz;
};

}
