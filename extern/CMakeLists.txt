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
