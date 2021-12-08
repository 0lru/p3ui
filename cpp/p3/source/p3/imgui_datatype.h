
#include <imgui.h>

namespace p3
{

    template<typename DataType> std::int32_t imgui_datatype;
    template<> std::int32_t imgui_datatype<std::int8_t> = ImGuiDataType_S8;
    template<> std::int32_t imgui_datatype<std::uint8_t> = ImGuiDataType_U8;
    template<> std::int32_t imgui_datatype<std::int16_t> = ImGuiDataType_S16;
    template<> std::int32_t imgui_datatype<std::uint16_t> = ImGuiDataType_U16;
    template<> std::int32_t imgui_datatype<std::int32_t> = ImGuiDataType_S32;
    template<> std::int32_t imgui_datatype<std::uint32_t> = ImGuiDataType_U32;
    template<> std::int32_t imgui_datatype<std::int64_t> = ImGuiDataType_S64;
    template<> std::int32_t imgui_datatype<std::uint64_t> = ImGuiDataType_U64;
    template<> std::int32_t imgui_datatype<float> = ImGuiDataType_Float;
    template<> std::int32_t imgui_datatype<double> = ImGuiDataType_Double;

}
