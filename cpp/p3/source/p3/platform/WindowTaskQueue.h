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

#include <p3/log.h>

namespace p3
{

    class Window::TaskQueue : public p3::TaskQueue
    {
    public:
        TaskQueue(Window* window)
            : _window(window)
        {
        }

        void run(Task&& task) final override
        {
            {
                std::lock_guard<std::mutex> l(_mutex);
                if (_closed)
                    throw std::runtime_error("closed");
                _tasks.push_back(std::move(task));
            }
            glfwPostEmptyEvent();
        }

        void process()
        {
            std::vector<Task> tasks;
            {
                std::lock_guard<std::mutex> l(_mutex);
                std::swap(_tasks, tasks);
                _tasks.clear();
            }
            for (auto& task : tasks)
            {
                try
                {
                    task();
                }
                catch (std::exception& e)
                {
                    log_fatal(fmt::format("uncaught exception/internal error:\n{}", e.what()));
                }
            }
        }

        void close()
        {
            {
                std::lock_guard<std::mutex> l(_mutex);
                _closed = true;
            }
            process();
        }

    private:
        Window* _window;
        std::mutex _mutex;
        std::vector<Task> _tasks;
        bool _closed = false;
    };

}
