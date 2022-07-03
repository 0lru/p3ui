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

extern std::function<void(std::function<void()>)> run_in_external_scope;

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

    ///
    /// If stop() is called while run_forever() is running, the loop will 
    /// run the current batch of callbacks and then exit.void stop();
    void stop();

    ///
    // the loop must not be running when this function is called.
    /// Any pending callbacks will be discarded.
    void close();

    /// 
    /// run until stopped / closed
    void run_forever();

    Queue const& queue() const { return _queue; }

    static std::shared_ptr<EventLoop> current();

private:
    std::mutex _mutex;
    Queue _queue;

    bool _stopped = true;
    bool _closed = false;

    std::vector<Observer*> _observer;
};

class EventLoop::Observer {
public:
    virtual ~Observer() = default;
    virtual void on_work_processed(EventLoop&) = 0;
};

}
