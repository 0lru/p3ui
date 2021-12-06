/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/
#pragma once

#define NOMINMAX
// #include <source_location>
#include <fmt/format.h>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <rang.hpp>

namespace logger
{

    namespace level
    {
        constexpr std::uint32_t none = 0;
        constexpr std::uint32_t fatal = 1;
        constexpr std::uint32_t error = 2;
        constexpr std::uint32_t warn = 3;
        constexpr std::uint32_t info = 4;
        constexpr std::uint32_t debug = 5;
        constexpr std::uint32_t verbose = 6;

        //        constexpr std::uint32_t current = 4;
#ifdef LOG_LEVEL
        constexpr std::uint32_t current = LOG_LEVEL;
#elif _DEBUG
        constexpr std::uint32_t current = 6;
#else
        constexpr std::uint32_t current = info;
#endif
    }

    /*template<typename ... Args>
    void log(std::source_location location, std::uint32_t level, std::string_view const text, Args&& ... args)
    {
        if (current == nullptr)
            return;
        current->log(std::move(location), level, fmt::format(text, std::forward<Args>(args)...));
    }*/

    inline std::string format(std::string_view filename, int line)
    {
        std::string lower(filename.begin(), filename.end());
        std::transform(lower.begin(), lower.end(), lower.begin(), [](auto c) { return std::tolower(c); });
        return fmt::format(" ({}:{})", std::filesystem::path(filename).stem().string(), line);
    }

    template<typename ... Args>
    inline void fatal(
        char const* filename,
        int line,
        const std::string_view text,
        Args&& ... args)
    {
        std::cout
            << rang::style::bold
            << rang::fg::red
            << rang::bg::black
            << "[fat!] "
            << fmt::format(text, std::forward<Args>(args)...)
            << logger::format(filename, line)
            << std::endl; // flush..
        exit(-1);
    }

    template<typename ... Args>
    inline void error(
        char const* filename,
        int line,
        const std::string_view text,
        Args&& ... args)
    {
        std::cout
            << rang::style::blink
            << rang::fg::red
            << rang::bg::black
            << "[err!] "
            << fmt::format(text, std::forward<Args>(args)...)
            << logger::format(filename, line)
            << std::endl; // flush..
        exit(-1);
    }

    template<typename ... Args>
    inline void warn(
        char const* filename,
        int line,
        std::string_view const text,
        Args&& ... args)
    {
        std::cout
            << rang::style::reset
            << rang::fg::yellow
            << rang::bg::black
            << "[warn] "
            << fmt::format(text, std::forward<Args>(args)...)
            << logger::format(filename, line)
            << std::endl; // flush..
    }

    template<typename ... Args>
    inline void debug(
        char const* filename,
        int line,
        std::string_view const text,
        Args&& ... args)
    {
        std::cout
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << "[dbug] "
            << rang::style::reset
            << rang::fg::reset
            << rang::bg::reset
            << fmt::format(text, std::forward<Args>(args)...)
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << logger::format(filename, line)
            << std::endl; // flush..
    }

    template<typename ... Args>
    inline void verbose(
        char const* filename,
        int line,
        std::string_view const text,
        Args&& ... args)
    {
        std::cout
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << "[verb] "
            << rang::style::reset
            << rang::fg::reset
            << rang::bg::reset
            << fmt::format(text, std::forward<Args>(args)...)
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << logger::format(filename, line)
            << std::endl; // flush..
    }

    template<typename ... Args>
    inline void info(
        char const* filename,
        int line,
        std::string_view const text,
        Args&& ... args)
    {
        std::cout
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << "[info] "
            << rang::style::reset
            << rang::fg::reset
            << rang::bg::reset
            << fmt::format(text, std::forward<Args>(args)...)
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << logger::format(filename, line)
            << std::endl; // flush..
    }

} // ~namespace logger

#define log_fatal(text, ...) if constexpr(::logger::level::current >= logger::level::fatal) ::logger::fatal(__FILE__, __LINE__, text, __VA_ARGS__)
#define log_error(text, ...) if constexpr(::logger::level::current >= logger::level::error) ::logger::error(__FILE__, __LINE__, text, __VA_ARGS__)
#define log_warn(text, ...) if constexpr(::logger::level::current >= logger::level::warn) ::logger::warn(__FILE__, __LINE__, text, __VA_ARGS__)
#define log_info(text, ...) if constexpr(::logger::level::current >= logger::level::info) ::logger::info(__FILE__, __LINE__, text, __VA_ARGS__)
#define log_debug(text, ...) if constexpr(::logger::level::current >= logger::level::debug) ::logger::debug(__FILE__, __LINE__, text, __VA_ARGS__)
#define log_verbose(text, ...) if constexpr(::logger::level::current >= logger::level::verbose) ::logger::verbose(__FILE__, __LINE__, text, __VA_ARGS__)
#define log_with_level(custom_level, text, ...) if constexpr(::logger::level::current >= custom_level) ::logger::info(__FILE__, __LINE__, text, __VA_ARGS__)
