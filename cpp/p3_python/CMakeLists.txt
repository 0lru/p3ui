file(GLOB SOURCES source/p3/python/*.cpp)
set(SOURCES ${SOURCES})
pybind11_add_module(p3ui ${SOURCES})
target_link_libraries(p3ui PUBLIC p3)

set_target_properties(p3ui
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)