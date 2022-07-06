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

std::function<void(std::function<void()>)> run_in_external_scope = [](std::function<void()> callback) {
    callback();
};

namespace {
    thread_local EventLoop* thread_local_event_loop = nullptr;
    thread_local std::thread::id thread_id;
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
    thread_id = std::this_thread::get_id();
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
    close();
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
    {
        std::lock_guard<std::mutex> l(_mutex);
        if (_closed)
            throw std::runtime_error("loop was closed already");
        _stopped = false;
    }
    while (true) {
        EventLoop::Work work;
        double timeout = 0.5;
        {
            std::lock_guard<std::mutex> l(_mutex);

            if (_stopped)
                break;
            auto now = Clock::now();
            while (!_queue.empty()) {
                auto diff = _queue.front().first - now;
                if (diff <= Duration::zero()) {
                    work.push_back(std::move(_queue.front().second));
                    std::pop_heap(_queue.begin(), _queue.end(), event_comparator());
                    _queue.pop_back();
                } else {
                    auto duration = _queue.front().first - now;
                    auto seconds = std::chrono::duration<double>(duration).count();
                    timeout = std::max(0., std::min(timeout, seconds));
                    break;
                }
            }
        }
        //
        // without lock do..
        if (work.empty()) {
            //
            // wait for 500ms or less time
            glfwWaitEventsTimeout(timeout);
        } else {
            //
            // (for python we need to acquire the gil)
            run_in_external_scope([&] {
                for (auto& item : work) {
                    try {
                        (*item)();
                    } catch (std::exception& e) {
                        log_error("error: {}", e.what());
                        throw;
                    }
                }
                work.clear();
            });
            glfwPollEvents();
        }
        //
        // render..
        for (auto observer : _observer)
            observer->on_work_processed(*this);
    }
}

void EventLoop::call_at(TimePoint time_point, std::unique_ptr<Event> event)
{
    {
        std::lock_guard<std::mutex> l(_mutex);
        if (_closed)
            throw std::runtime_error("cannot schedule task on a closed loop");
        _queue.emplace_back(time_point, std::move(event));
        std::push_heap(_queue.begin(), _queue.end(), event_comparator());
    }
    //
    // https://stackoverflow.com/questions/17101922/do-i-have-to-acquire-lock-before-calling-condition-variable-notify-one
    glfwPostEmptyEvent();
    // auto tid = std::this_thread::get_id();
}

void EventLoop::close()
{
    EventLoop::Queue temp;
    log_debug("closing event loop");
    {
        std::lock_guard<std::mutex> l(_mutex);
        if (!_stopped)
            log_fatal("cannot close a running loop");
        std::swap(_queue, temp);
        _closed = true;
    }
    if (!temp.empty()) 
    { 
        run_in_external_scope([&] {
            temp.clear();
        });
    }
}

void EventLoop::stop()
{
    {
        std::lock_guard<std::mutex> l(_mutex);
        _stopped = true;
    }
    //
    // make sure thread will process the change of the flag
    glfwPostEmptyEvent();
}

}
