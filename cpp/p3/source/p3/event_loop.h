#pragma once

#include <functional>
#include <vector>

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace p3 {

/*
 * this mimics an asyncio python event loop. the purpose
 * is to provide a decent implementation that provides the possibility
 * to hook in the render-action right after all "immediate" work was
 * processed by the loop.
 */
class EventLoop {
public:
    using Event = std::function<void()>;
    using Clock = std::chrono::high_resolution_clock;
    using Duration = Clock::duration;
    using TimePoint = Clock::time_point;
    using ScheduledEvent = std::pair<TimePoint, Event>;
    using Queue = std::vector<ScheduledEvent>;
    using Work = std::vector<Event>;

    void call_at(TimePoint, Event);
    void close();

    void run_forever();

    Queue const& queue() const { return _queue; }

private:
    Work pop_work_from_queue();

    std::mutex _mutex;
    std::condition_variable _condition;
    Queue _queue;
    bool _closed = false;
};

}
