project(vulkan_glfw_wrappers VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE INC "include/*.hh")
file(GLOB_RECURSE SRC "src/*.cc")

add_library(vulkan_glfw_wrappers)
target_sources(vulkan_glfw_wrappers PRIVATE ${INC} ${SRC})
target_include_directories(vulkan_glfw_wrappers PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(vulkan_glfw_wrappers
    PUBLIC
    vulkan_common
    GSL
    PRIVATE
    vulkan_copts_common
    fmt
    glfw)

configure_lto(vulkan_glfw_wrappers)
configure_tidy(vulkan_glfw_wrappers)

