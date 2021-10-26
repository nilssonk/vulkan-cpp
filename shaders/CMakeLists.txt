project(vulkan_shaders VERSION 0.1 LANGUAGES GLSL)

file(GLOB SRC *.glsl)

add_library(vulkan_shaders OBJECT ${SRC})
target_compile_options(vulkan_shaders PRIVATE -O)

add_custom_target(vulkan_shaders_copy ALL)
add_dependencies(vulkan_shaders_copy vulkan_shaders)

# Copy object files to the binary directory
# Note:
# Generator expressions like $<TARGET_OBJECTS:vulkan_shaders> are not iterable at
# configure time, so the COMMAND_EXPAND_LISTS option, ostensibly introduced in
# CMake 3.8, must be given
add_custom_command(
    TARGET vulkan_shaders_copy
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:vulkan_shaders> ${CMAKE_CURRENT_BINARY_DIR}/
    COMMAND_EXPAND_LISTS)
