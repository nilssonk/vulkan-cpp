# Thanks to Florian on Stackoverflow for giving an extremely useful answer!
# https://stackoverflow.com/questions/38293535/generic-rule-from-makefile-to-cmake

if(NOT DEFINED Vulkan_GLSLC_EXECUTABLE)
    message(FATAL_ERROR "GLSLC must be present for GLSL compilation")
endif()

# Find the compiler
set(CMAKE_GLSL_COMPILER ${Vulkan_GLSLC_EXECUTABLE})
mark_as_advanced(CMAKE_GLSL_COMPILER)

set(CMAKE_GLSL_SOURCE_FILE_EXTENSIONS glsl;GLSL)
set(CMAKE_GLSL_OUTPUT_EXTENSION .spv)
set(CMAKE_GLSL_COMPILER_ENV_VAR "GLSLC")

# Configure variables set in this file for fast reload later on
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeGLSLCompiler.cmake.in
               ${CMAKE_PLATFORM_INFO_DIR}/CMakeGLSLCompiler.cmake)