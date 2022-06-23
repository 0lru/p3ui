#pragma once

#include <functional>
#include <vector>

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace p3 {

class Event {
public:
    virtual ~Event() { }
    virtual void operator()() = 0;
    static std::unique_ptr<Event> create(std::function<void()>);
};

/*
 * each window requires an event loop. this loop can be shared
 * by multiple windows.
 */
class EventLoop : public std::enable_shared_from_this<EventLoop> {
public:
    using Clock = std::chrono::high_resolution_clock;
    using Duration = Clock::duration;
    using TimePoint = Clock::time_point;
    using ScheduledEvent = std::pair<TimePoint, std::unique_ptr<Event>>;
    using Queue = std::vector<ScheduledEvent>;
    using Work = std::vector<std::unique_ptr<Event>>;

    class Observer;
    void add_observer(Observer* observer) { _observer.push_back(observer); }
    void remove_observer(Observer* observer)
    {
        _observer.erase(std::remove_if(_observer.begin(),
                            _observer.end(), [&](auto item) {
                                return item == observer;
                            }),
            _observer.end());
    }

    EventLoop();
    ~EventLoop();

    void call_at(TimePoint, std::unique_ptr<Event>);
    void stop();
    void close();

    void run_forever();

    Queue const& queue() const { return _queue; }

    static std::shared_ptr<EventLoop> current();

private:
    Work pop_work_from_queue();

    std::mutex _mutex;
    std::condition_variable _condition;
    Queue _queue;
    bool _closed = false;
    bool _stopped = false;
    std::vector<Observer*> _observer;
};

class EventLoop::Observer {
public:
    virtual ~Observer() = default;
    virtual void on_work_processed(EventLoop&) = 0;
};

}
