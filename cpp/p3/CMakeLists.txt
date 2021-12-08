file(GLOB SOURCES source/p3/*.cpp source/p3/backend/*.cpp source/p3/platform/*.cpp)
add_library(p3 STATIC ${SOURCES})
target_include_directories(p3 PUBLIC source/)
target_link_libraries(p3 
    PUBLIC
    lib_skia
    glad
    imgui
    implot
    glfw
    rang
    pugixml
    p3_parser
    fmt-header-only
    ${OPENGL_LIBRARIES})

add_subdirectory(tests)
