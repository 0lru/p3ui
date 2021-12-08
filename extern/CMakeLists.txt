#
# ----- catch2 -----------------------------------------------------------------
set(CATCH_BUILD_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
add_subdirectory(catch2)

#
# ----- fmtlib -----------------------------------------------------------------
add_subdirectory(fmtlib)

#
# ----- glad -------------------------------------------------------------------
add_subdirectory(glad)
# add_library(glad INTERFACE)
# target_include_directories(glad INTERFACE glad/source)

#
# ----- imgui ------------------------------------------------------------------
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

#
# ----- impplot ----------------------------------------------------------------
set(SOURCES implot/implot.cpp
#            implot/implot_demo.cpp
            implot/implot_items.cpp)
add_library(implot STATIC ${SOURCES})
target_include_directories(implot PUBLIC implot/)
target_link_libraries(implot PUBLIC imgui)

#
# ----- pybind11 ---------------------------------------------------------------
add_subdirectory(pybind11)

#
# ----- glfw -------------------------------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

#
# ----- rang -------------------------------------------------------------------
add_library(rang INTERFACE)
target_include_directories(rang INTERFACE rang/include)

#
# ----- pugixml ----------------------------------------------------------------
add_library(pugixml "pugixml/src/pugixml.cpp")
target_include_directories(pugixml PUBLIC pugixml/src/)

#
# ----- skia (download) --------------------------------------------------------
if(WIN32)
  set(SKIA_LIB_FILENAME "${CMAKE_CURRENT_SOURCE_DIR}/skia/out/Release/skia.lib")
else()
  set(SKIA_LIB_FILENAME "${CMAKE_CURRENT_SOURCE_DIR}/skia/out/Release/skia.a")
endif()

# @see https://gitlab.kitware.com/cmake/cmake/-/issues/21444
add_custom_command(
    OUTPUT "${SKIA_LIB_FILENAME}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/skia.py"
    COMMAND python skia.py
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
add_custom_target(lib_skia_download DEPENDS "${SKIA_LIB_FILENAME}")

#
# ----- skia -------------------------------------------------------------------
if(WIN32)
  file(GLOB SKIA_SVG_OBJECTS skia/out/Release/obj/experimental/svg/model/*.obj)
else()
  file(GLOB SKIA_SVG_OBJECTS skia/out/Release/obj/experimental/svg/model/*.o)
endif()
set(SKIA_SVG_OBJECTS ${SKIA_SVG_OBJECTS})

#
# per default, scope is local, thus "global"
add_library(lib_skia STATIC IMPORTED GLOBAL)
add_dependencies(lib_skia lib_skia_download)
set_property(TARGET lib_skia PROPERTY IMPORTED_LOCATION "${SKIA_LIB_FILENAME}")
set_property(TARGET lib_skia PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/skia")
set_property(TARGET lib_skia PROPERTY INTERFACE_COMPILE_DEFINITIONS SK_GL)
if(WIN32)
  set_property(TARGET lib_skia PROPERTY INTERFACE_LINK_LIBRARIES ${SKIA_SVG_OBJECTS} Gdi32 OpenGL32 Usp10 User32 OleAut32 Ole32 FontSub)
  set_property(TARGET lib_skia PROPERTY INTERFACE_COMPILE_OPTIONS /Zc:inline /wd4244 /wd4996 /wd4267 /wd4180 /MD)
endif()

#
# ----- python-skia ------------------------------------------------------------
file(GLOB SOURCES python-skia/src/skia/*.cpp)
pybind11_add_module(skia ${SOURCES})
target_link_libraries(skia PUBLIC lib_skia)
# target_precompile_headers(skia PRIVATE "python-skia/src/skia/common.h")
if(WIN32)
  target_link_options(skia PUBLIC /OPT:ICF /OPT:REF) # remove unreferenced symbols, one iteration
else()
  #TODO
endif()
