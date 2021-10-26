# Thanks to Florian on Stackoverflow for giving an extremely useful answer!
# https://stackoverflow.com/questions/38293535/generic-rule-from-makefile-to-cmake

if(NOT CMAKE_GLSL_COMPILE_OBJECT)
    set(CMAKE_GLSL_COMPILE_OBJECT "<CMAKE_GLSL_COMPILER> <FLAGS> -o <OBJECT> <SOURCE>")
endif()
set(CMAKE_GLSL_INFORMATION_LOADED 1)