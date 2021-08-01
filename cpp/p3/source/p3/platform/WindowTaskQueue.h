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
                _tasks.push_back(std::move(task));
               glfwPostEmptyEvent();
            }
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
                    log_fatal(std::format("uncaught exception/internal error:\n{}", e.what()));
                }
            }
            tasks.clear();
        }

    private:
        Window* _window;
        std::mutex _mutex;
        std::vector<Task> _tasks;
    };

}
