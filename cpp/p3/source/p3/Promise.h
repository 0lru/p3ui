#pragma once

#include <functional>
#include <memory>
#include <future>

namespace p3
{

    //
    // abstract, moveable promise. using this instead of std::promise/future
    // to avoid the creation of additional, possibly detached threads.
    template<typename T>
    class Promise
    {
    public:
        class Implementation
        {
        public:
            virtual ~Implementation() = default;
            virtual void set_value(T) = 0;
            virtual void set_exception(std::exception_ptr) = 0;
        };

        Promise(std::shared_ptr<Implementation> implementation)
            : _implementation(std::move(implementation))
        {
        }

        void set_value(T t)
        {
            _implementation->set_value(std::move(t));
        }

        void set_exception(std::exception_ptr e)
        {
            _implementation->set_exception(std::move(e));
        }

    private:
        std::shared_ptr<Implementation> _implementation;
    };

    template<>
    class Promise<void>
    {
    public:
        class Implementation
        {
        public:
            virtual ~Implementation() = default;
            virtual void set_value() = 0;
            virtual void set_exception(std::exception_ptr) = 0;
        };

        Promise(std::shared_ptr<Implementation> implementation = nullptr)
            : _implementation(std::move(implementation))
        {
        }

        void set_value()
        {
            if (_implementation)
                _implementation->set_value();
        }

        void set_exception(std::exception_ptr e)
        {
            if (_implementation)
                _implementation->set_exception(std::move(e));
        }

    private:
        std::shared_ptr<Implementation> _implementation = nullptr;
    };


    namespace detail
    {
        template<typename Result>
        struct SetTaskResult
        {
            template<typename F>
            void operator()(std::promise<Result>& promise, F& task)
            {
                promise.set_value(task());
            }
        };

        template<>
        struct SetTaskResult<void>
        {
            template<typename F>
            void operator()(std::promise<void>& promise, F& task)
            {
                task();
                promise.set_value();
            }
        };
    }

    class TaskQueue
    {
    public:
        class Driven
        {
        public:
            virtual ~Driven() = default;
            virtual std::shared_ptr<TaskQueue> const& task_queue() const = 0;
        };

        static std::shared_ptr<TaskQueue> const DirectCall;

        using Task = std::function<void()>;
        virtual ~TaskQueue() = default;

        virtual void run(Task&&) = 0;

        template<typename T>
        auto run_blocking_task(T&& task) -> decltype(task())
        {
            auto promise = std::make_shared<std::promise<decltype(task())>>();
            auto future = promise->get_future();
            run([task{ std::move(task) }, promise]() mutable {
                try
                {
                    detail::SetTaskResult<decltype(task())>()(*promise, task);
                }
                catch (...)
                {
                    promise->set_exception(std::current_exception());
                }
            });
            return future.get();
        }
    };

}
