#include "TaskQueue.h"

namespace p3
{

    class DirectCall : public TaskQueue
    {
    public:
        DirectCall() = default;

        virtual void run(Task&& task)
        {
            task();
        }
    };

    namespace
    {
        auto direct_call = std::make_shared<DirectCall>();
    }

    std::shared_ptr<TaskQueue> const TaskQueue::DirectCall = direct_call;

}