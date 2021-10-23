project(vulkan_vulkan_wrappers VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE INC "include/*.hh")
file(GLOB_RECURSE SRC "src/*.cc")

add_library(vulkan_vulkan_wrappers)
target_sources(vulkan_vulkan_wrappers PRIVATE ${INC} ${SRC})
target_include_directories(vulkan_vulkan_wrappers PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(vulkan_vulkan_wrappers
    PUBLIC
    vulkan_common
    GSL
    vulkan
    PRIVATE
    vulkan_copts_common
    vulkan_glfw_wrappers
    fmt)

configure_lto(vulkan_vulkan_wrappers)
configure_tidy(vulkan_vulkan_wrappers)

