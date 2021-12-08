#pragma once
#include "p3ui.h"

#include <p3/TaskQueue.h>

namespace p3::python
{

    class AsyncTaskQueue : public TaskQueue
    {
    public:
        AsyncTaskQueue(py::object loop) : _loop(std::move(loop))
        {
        }

        void run(Task&& task) final override
        {
            py::gil_scoped_acquire acquire;
            _loop.attr("call_soon_threadsafe")(py::cpp_function([task{std::move(task)}]() {
                py::gil_scoped_release release;
                task();
            }));
        }

    private:
        py::object _loop;
    };

}
