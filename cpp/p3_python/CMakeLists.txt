file(GLOB SOURCES
    source/p3/python/*.cpp
)
pybind11_add_module(_p3ui ${SOURCES})
target_link_libraries(_p3ui PUBLIC p3)

set_target_properties(_p3ui
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)