#include <catch2/catch.hpp>

#include <p3/event_loop.h>

namespace p3::tests {

TEST_CASE("can_insert_to_event_loop", "[p3]")
{
    EventLoop loop;
    loop.call_at(EventLoop::Clock::now(), []() {});
    REQUIRE(!loop.queue().empty());
}

TEST_CASE("most_recent_event_gets_heap_sorted_to_top_element", "[p3]")
{
    auto t1 = EventLoop::Clock::now();
    auto t2 = t1 + std::chrono::seconds(1);
    auto t3 = t2 + std::chrono::seconds(1);
    EventLoop loop;
    loop.call_at(t2, []() {});
    loop.call_at(t1, []() {});
    loop.call_at(t3, []() {});
    REQUIRE(loop.queue().front().first == t1);
}

TEST_CASE("work_gets_processed_in_order", "[p3]")
{
    std::vector<int> result;
    auto t1 = EventLoop::Clock::now();
    auto t2 = t1 - std::chrono::seconds(1);
    auto t3 = t2 - std::chrono::seconds(1);
    EventLoop loop;
    loop.call_at(t2, [&]() { result.push_back(2); });
    loop.call_at(t1, [&]() { result.push_back(1); loop.close(); });
    loop.call_at(t3, [&]() { result.push_back(3); });
    REQUIRE(loop.queue().front().first == t3);
    loop.run_forever();
    REQUIRE(result == std::vector<int>{ 3, 2, 1 });
}

TEST_CASE("work_gets_processed_in_time_approx", "[p3]")
{
    auto t1 = EventLoop::Clock::now();
    auto t2 = t1 + std::chrono::milliseconds(2);
    EventLoop loop;
    loop.call_at(t2, [&]() { loop.close(); });
    loop.call_at(t1, [&]() { });
    loop.run_forever();
    REQUIRE(loop.queue().empty());
    REQUIRE(EventLoop::Clock::now() > t2);
}

}
