file(GLOB SOURCES source/p3/python/*.cpp)
set(SOURCES ${SOURCES})
pybind11_add_module(p3ui ${SOURCES})
target_link_libraries(p3ui PUBLIC p3)
