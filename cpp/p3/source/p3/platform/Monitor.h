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
#pragma once

#include "VideoMode.h"

#include <memory>
#include <vector>

namespace p3
{

    class Window;

    class Monitor
        : public Synchronizable
    {
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
