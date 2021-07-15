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
