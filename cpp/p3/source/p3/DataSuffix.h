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
