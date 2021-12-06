set(CATCH_BUILD_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
add_subdirectory(catch2)

add_subdirectory(fmtlib)

add_subdirectory(glad)
# add_library(glad INTERFACE)
# target_include_directories(glad INTERFACE glad/source)


set(SOURCES imgui/imgui.cpp
            imgui/imgui_draw.cpp
            imgui/imgui_demo.cpp
            imgui/imgui_tables.cpp
            imgui/imgui_widgets.cpp
            imgui/backends/imgui_impl_glfw.cpp
            imgui/backends/imgui_impl_opengl2.cpp
            imgui/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${SOURCES})
target_include_directories(imgui PUBLIC imgui/)
target_link_libraries(imgui PUBLIC glfw glad)

set(SOURCES implot/implot.cpp
#            implot/implot_demo.cpp
            implot/implot_items.cpp)
add_library(implot STATIC ${SOURCES})
target_include_directories(implot PUBLIC implot/)
target_link_libraries(implot PUBLIC imgui)

#
# this is header-only, but adds cmake definitions
add_subdirectory(pybind11)

#
# glfw for now, maybe vulkan later
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

add_library(rang INTERFACE)
target_include_directories(rang INTERFACE rang/include)

#
# xml parser
add_library(pugixml "pugixml/src/pugixml.cpp")
target_include_directories(pugixml PUBLIC pugixml/src/)

#
# prebuild skia
add_library(libskia STATIC IMPORTED)
if (WIN32)
# TODO: USE "IMPORTED_LIBNAME"
  set_target_properties(libskia PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/skia/out/Release/skia.lib")
else()
  set_target_properties(libskia PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/skia/out/Release/skia.a")
endif()
set_target_properties(libskia PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/skia")


if(WIN32)
  file(GLOB SKIA_SVG_OBJECTS skia/out/Release/obj/experimental/svg/model/*.obj)
else()
  file(GLOB SKIA_SVG_OBJECTS skia/out/Release/obj/experimental/svg/model/*.o)
endif()
set(SKIA_SVG_OBJECTS ${SKIA_SVG_OBJECTS})

# skia python
#
# todo: zip skia.h
file(GLOB SOURCES python-skia/src/skia/*.cpp)
set(SOURCES ${SOURCES})
pybind11_add_module(skia ${SOURCES})

if(WIN32)

  target_compile_features(skia PRIVATE cxx_std_17)
  target_compile_definitions(skia PUBLIC SK_GL)
  target_compile_options(skia PUBLIC /Zc:inline /wd4244 /wd4996 /wd4267 /wd4180 /MD)

  target_link_libraries(skia PUBLIC libskia ${SKIA_SVG_OBJECTS} Gdi32 OpenGL32 Usp10 User32 OleAut32 Ole32 FontSub)
  target_link_options(skia PUBLIC /OPT:ICF /OPT:REF)

endif()
