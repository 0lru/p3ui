file(GLOB SOURCES p3/*.cpp)
set(SOURCES ${SOURCES})
add_library(p3 STATIC ${SOURCES})
target_link_libraries(p3 PUBLIC glad imgui implot glfw ${OPENGL_LIBRARIES})

file(GLOB SOURCES p3/python/*.cpp)
set(SOURCES ${SOURCES})
pybind11_add_module(p3ui ${SOURCES})
target_link_libraries(p3ui PUBLIC p3)

# set(SOURCES examples/widgets/widgets.cpp)
# add_executable(widgets ${SOURCES})
# target_link_libraries(widgets liboogui)
