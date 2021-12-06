project(p3ui) 
cmake_minimum_required(VERSION 3.5)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

# set(PYTHON_EXECUTABLE "")

find_package(OpenGL)

add_subdirectory(extern)

add_subdirectory(cpp)

set_target_properties(_p3ui
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/p3ui"
)
