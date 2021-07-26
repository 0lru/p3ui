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

#include <imgui.h>

namespace p3
{

    template<typename DataType> std::int32_t DataSuffix;
    template<> std::string DataSuffix<std::int8_t> = std::string("S8");
    template<> std::string DataSuffix<std::uint8_t> = std::string("U8");
    template<> std::string DataSuffix<std::int16_t> = std::string("S16");
    template<> std::string DataSuffix<std::uint16_t> = std::string("U16");
    template<> std::string DataSuffix<std::int32_t> = std::string("S32");
    template<> std::string DataSuffix<std::uint32_t> = std::string("U32");
    template<> std::string DataSuffix<std::int64_t> = std::string("S64");
    template<> std::string DataSuffix<std::uint64_t> = std::string("U64");
    template<> std::string DataSuffix<float> = std::string("Float");
    template<> std::string DataSuffix<double> = std::string("Double");

    template<template<typename> typename F, typename ... Args>
    void invoke_for_all_data_types(Args&&... args)
    {
        F<std::int8_t>()(std::forward<Args>(args)...);
        F<std::uint8_t>()(std::forward<Args>(args)...);
        F<std::int16_t>()(std::forward<Args>(args)...);
        F<std::uint16_t>()(std::forward<Args>(args)...);
        F<std::int32_t>()(std::forward<Args>(args)...);
        F<std::uint32_t>()(std::forward<Args>(args)...);
        F<std::int64_t>()(std::forward<Args>(args)...);
        F<std::uint64_t>()(std::forward<Args>(args)...);
        F<float>()(std::forward<Args>(args)...);
        F<double>()(std::forward<Args>(args)...);
    }

}
