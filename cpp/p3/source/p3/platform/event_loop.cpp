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
    while (true) {
        EventLoop::Work work;
        double timeout=0.5;
        {
            //
            // locked..
            std::lock_guard<std::mutex> l(_mutex);
            if (_closed)
                _queue.clear();
            if (_stopped)
                break;
            while (!_queue.empty() && _queue.front().first - Clock::now() <= Duration::zero()) {
                work.push_back(std::move(_queue.front().second));
                std::pop_heap(_queue.begin(), _queue.end(), event_comparator());
                _queue.pop_back();
            }
        }

        //
        // at this point glfw events are already pushed
        for (auto& w : work)
            w->operator()();
        work.clear();

        //
        // glfw updated and work is done.. render
        for (auto observer : _observer)
            observer->on_work_processed(*this);

        //
        // probably optimizable... (only one lock required)
        {
            std::lock_guard<std::mutex> l(_mutex);
            if (!_queue.empty() && _queue.front().first - Clock::now() > Duration::zero()) {
                auto duration = _queue.front().first - Clock::now();
                auto seconds = std::chrono::duration<double>(duration).count();
                timeout = std::max(0., std::min(timeout, seconds));
            }
        }

        glfwWaitEventsTimeout(timeout);
    }
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
    glfwPostEmptyEvent();
}

void EventLoop::close()
{
    log_info("closing event loop");
    {
        std::lock_guard<std::mutex> l(_mutex);
        _stopped = true;
        _closed = true;
    }
    glfwPostEmptyEvent();
}

void EventLoop::stop()
{
    {
        std::lock_guard<std::mutex> l(_mutex);
        _stopped = true;
    }
    glfwPostEmptyEvent();
}

}
