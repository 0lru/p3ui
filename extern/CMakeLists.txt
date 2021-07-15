set(SOURCES imgui/imgui.cpp
            imgui/imgui_draw.cpp
            imgui/imgui_demo.cpp
            imgui/imgui_tables.cpp
            imgui/imgui_widgets.cpp
            imgui/backends/imgui_impl_glfw.cpp
            imgui/backends/imgui_impl_opengl2.cpp)
add_library(imgui STATIC ${SOURCES})

set(SOURCES implot/implot.cpp
#            implot/implot_demo.cpp
            implot/implot_items.cpp)
add_library(implot STATIC ${SOURCES})

#
# this is header-only, but adds cmake definitions
add_subdirectory(pybind11)

#
# glfw for now, maybe vulkan later
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

#
# not sure if this is really needed
add_subdirectory(glad)
