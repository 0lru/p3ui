file(GLOB SOURCES source/p3/*.cpp source/p3/backend/*.cpp)
set(SOURCES ${SOURCES})
add_library(p3 STATIC ${SOURCES})
target_include_directories(p3 PUBLIC source/)
target_link_libraries(p3 
    PUBLIC 
    glad 
    imgui 
    implot 
    glfw 
    rang
    pugixml
    ${OPENGL_LIBRARIES})

add_subdirectory(tests)