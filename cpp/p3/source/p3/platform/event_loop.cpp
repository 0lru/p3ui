#include "event_loop.h"

#define NOMINMAX
#include "Window.h"

#define GLFW_INCLUDE_NONE
#pragma warning(push)
#pragma warning(disable : 4005)
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#pragma warning(pop)

#include <p3/log.h>

#include <algorithm>
#include <iostream>

namespace {

struct event_comparator {
    template <typename T>
    bool operator()(const T& l, const T& r) const { return l.first > r.first; }
};

}

namespace p3 {

namespace {
    thread_local EventLoop* thread_local_event_loop = nullptr;
}

std::shared_ptr<EventLoop> EventLoop::current()
{
    if (thread_local_event_loop == nullptr)
        return nullptr;
    return thread_local_event_loop->shared_from_this();
}

EventLoop::EventLoop()
{
    thread_local_event_loop = this;
    //
    // assumes that the event loop belongs to the current thread
    if (!glfwInit())
        log_fatal("could not init glfw");
    //
    // terminate on error
    glfwSetErrorCallback([](int code, char const* text) {
        log_fatal("glfw error, code={}, text=\"{}\"", code, text);
    });
}

EventLoop::~EventLoop()
{
    //
    // terminate after any window has been destroyed
    glfwTerminate();
}

EventLoop::Work EventLoop::pop_work_from_queue()
{
    EventLoop::Work work;
    std::unique_lock<std::mutex> l(_mutex);
    while (!_stopped) {

        glfwPollEvents();
        
        if (_queue.empty()) {
            _condition.wait_until(l, Clock::now() + std::chrono::milliseconds(500));
            continue;
        }
        
        if (_queue.front().first - Clock::now() > Duration::zero()) {
            _condition.wait_until(l, _queue.front().first);
            continue;
        }
        
        while (!_queue.empty() && _queue.front().first - Clock::now() <= Duration::zero()) {
            work.push_back(std::move(_queue.front().second));
            //
            // pop heap moves effectively to the back
            std::pop_heap(_queue.begin(), _queue.end(), event_comparator());
            _queue.pop_back();
        }

        return work;
    }
    return work;
}

class FunctionalEvent : public Event {
public:
    FunctionalEvent(std::function<void()> function)
        : _function(std::move(function))
    {
    }

    virtual void operator()()
    {
        _function();
    };

private:
    std::function<void()> _function;
};

std::unique_ptr<Event> Event::create(std::function<void()> function)
{
    return std::make_unique<FunctionalEvent>(std::move(function));
}

void EventLoop::run_forever()
{
    auto work = pop_work_from_queue();
    do {
        for (auto& task : work)
            task->operator()();
        for (auto observer : _observer)
            observer->on_work_processed(*this);
        work = pop_work_from_queue();
    } while (!work.empty());
}

void EventLoop::call_at(TimePoint time_point, std::unique_ptr<Event> event)
{
    {
        std::lock_guard<std::mutex> l(_mutex);
        if (_stopped || _closed)
            return;
        _queue.emplace_back(time_point, std::move(event));
        std::push_heap(_queue.begin(), _queue.end(), event_comparator());
    }
    //
    // https://stackoverflow.com/questions/17101922/do-i-have-to-acquire-lock-before-calling-condition-variable-notify-one
    _condition.notify_all();
}

void EventLoop::close()
{
    {
        std::lock_guard<std::mutex> l(_mutex);
        _stopped = true;
        _closed = true;
    }
    _condition.notify_all();
}

void EventLoop::stop()
{
    {
        std::lock_guard<std::mutex> l(_mutex);
        _stopped = true;
    }
    _condition.notify_all();
}

}
