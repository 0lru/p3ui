#include "event_loop.h"

#include <algorithm>
#include <iostream>

namespace {

struct event_comparator {
    template <typename T>
    bool operator()(const T& l, const T& r) const
    {
        return l.first > r.first;
    }
};

}

namespace p3 {

EventLoop::Work EventLoop::pop_work_from_queue()
{
    EventLoop::Work work;
    std::unique_lock<std::mutex> l(_mutex);
    while (!_closed) {
        if (_queue.empty()) {
            _condition.wait(l);
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

void EventLoop::process_work(Work work)
{
    for (auto& task : work) {
        task();
    }
}

void EventLoop::run_forever()
{
    auto work = pop_work_from_queue();
    do {
        process_work(std::move(work));
        for (auto observer : _observer)
            observer->on_work_processed(*this);
        work = pop_work_from_queue();
    } while (!work.empty());
}

void EventLoop::call_at(TimePoint time_point, Event event)
{
    {
        std::lock_guard<std::mutex> l(_mutex);
        if (_closed)
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
        _closed = true;
    }
    _condition.notify_all();
}

}
