#pragma once

#include <chrono>

namespace p3
{

    class Timer
    {
    public:
        std::chrono::nanoseconds time_and_reset()
        {
            auto now = Timer::now();
            auto delta = now - _timepoint;
            _timepoint = now;
            return delta;
        }

        void reset()
        {
            _timepoint = now();
        }

        std::chrono::nanoseconds time() const
        {
            return now() - _timepoint;
        }

        inline static std::chrono::steady_clock::time_point now()
        {
            return std::chrono::steady_clock::now();
        }

    private:
        std::chrono::steady_clock::time_point _timepoint = Timer::now();
    };

}
