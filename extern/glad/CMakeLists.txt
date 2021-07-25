# list:
# GL_ARB_debug_output%2CGL_ARB_fragment_shader%2CGL_ARB_framebuffer_object%2CGL_ARB_shader_objects%2CGL_ARB_vertex_shader
add_library(glad STATIC src/gl.c)
target_include_directories(glad PUBLIC include/)
