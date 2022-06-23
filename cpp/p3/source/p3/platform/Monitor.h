#pragma once

#include "VideoMode.h"

#include <memory>
#include <string>
#include <vector>

namespace p3 {

class Window;

class Monitor {
public:
    Monitor() = default;
    Monitor(Monitor const&) = default;
    Monitor(std::shared_ptr<Window>, GLFWmonitor*);

    bool operator==(Monitor const&) const;
    bool operator!=(Monitor const& monitor) const { return !(*this == monitor); }

    VideoMode mode() const;
    void set_mode(VideoMode);

    std::vector<VideoMode> modes() const;

    std::string name() const;

    double dpi() const;

private:
    std::weak_ptr<Window> _window;
    GLFWmonitor* _handle = nullptr;
};

}
