project(p3ui) 
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif(MSVC)

# set(PYTHON_EXECUTABLE "")

find_package(OpenGL)

add_subdirectory(extern)

add_subdirectory(cpp)

set_target_properties(p3ui
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
