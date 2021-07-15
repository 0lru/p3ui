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

#include <source_location>
#include <format>
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
#ifdef LOG_LEVEL
        constexpr std::uint32_t current = LOG_LEVEL;
#elif _DEBUG
        constexpr std::uint32_t current = verbose;
#else
        constexpr std::uint32_t current = verbose;
#endif
    }

    /*
    template<typename ... Args>
    void log(std::source_location location, std::uint32_t level, std::string_view const text, Args&& ... args)
    {
        if (current == nullptr)
            return;
        current->log(std::move(location), level, std::format(text, std::forward<Args>(args)...));
    }*/

    inline std::string __log_format(std::source_location location)
    {
        std::string file_name = location.file_name();
        std::transform(file_name.begin(), file_name.end(), file_name.begin(), [](auto c) { return std::tolower(c); });
        return std::format(" ({}:{}:{})",
            std::filesystem::path(file_name).stem().string(),
            location.line(),
            location.function_name());
    }

    template<typename ... Args>
    inline void fatal(
        std::source_location const location, 
        const std::string_view text, 
        Args&& ... args)
    {
        std::cout
            << rang::style::bold
            << rang::fg::red
            << rang::bg::black
            << "[fat!] "
            << std::format(text, std::forward<Args>(args)...)
            << __log_format(location)
            << std::endl; // flush..
        exit(-1);
    }

    template<typename ... Args>
    inline void error(
        std::source_location const location, 
        const std::string_view text, 
        Args&& ... args)
    {
        std::cout
            << rang::style::blink
            << rang::fg::red
            << rang::bg::black
            << "[err!] "
            << std::format(text, std::forward<Args>(args)...)
            << __log_format(location)
            << std::endl; // flush..
        exit(-1);
    }

    template<typename ... Args>
    inline void warn(
        std::source_location const location, 
        std::string_view const text, 
        Args&& ... args)
    {
        std::cout
            << rang::style::reset
            << rang::fg::yellow
            << rang::bg::black
            << "[warn] "
            << std::format(text, std::forward<Args>(args)...)
            << __log_format(location)
            << std::endl; // flush..
    }

    template<typename ... Args>
    inline void debug(
        std::source_location const location, 
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
            << std::format(text, std::forward<Args>(args)...)
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << __log_format(location)
            << std::endl; // flush..
    }

    template<typename ... Args>
    inline void verbose(
        std::source_location const location, 
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
            << std::format(text, std::forward<Args>(args)...)
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << __log_format(location)
            << std::endl; // flush..
    }

    template<typename ... Args>
    inline void info(
        std::source_location const location, 
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
            << std::format(text, std::forward<Args>(args)...)
            << rang::style::bold
            << rang::fg::gray
            << rang::bg::black
            << __log_format(location)
            << std::endl; // flush..
    }
}

#define log_fatal(text, ...) if constexpr(::logger::level::current >= logger::level::fatal) ::logger::fatal(std::source_location::current(), text, __VA_ARGS__)
#define log_error(text, ...) if constexpr(::logger::level::current >= logger::level::error) ::logger::error(std::source_location::current(), text, __VA_ARGS__)
#define log_warn(text, ...) if constexpr(::logger::level::current >= logger::level::warn) ::logger::warn(std::source_location::current(), text, __VA_ARGS__)
#define log_info(text, ...) if constexpr(::logger::level::current >= logger::level::info) ::logger::info(std::source_location::current(), text, __VA_ARGS__)
#define log_debug(text, ...) if constexpr(::logger::level::current >= logger::level::debug) ::logger::debug(std::source_location::current(), text, __VA_ARGS__)
#define log_verbose(text, ...) if constexpr(::logger::level::current >= logger::level::verbose) ::logger::verbose(std::source_location::current(), text, __VA_ARGS__)
#define log_with_level(custom_level, text, ...) if constexpr(::logger::level::current >= custom_level) ::logger::info(std::source_location::current(), text, __VA_ARGS__)
