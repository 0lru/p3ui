file(GLOB SOURCES
    source/p3/python/*.cpp
)
pybind11_add_module(native ${SOURCES})
target_link_libraries(native PUBLIC p3)

set_target_properties(native
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)