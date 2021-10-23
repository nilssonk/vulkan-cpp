project(vulkan_main VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE INC "include/*.hh")
file(GLOB_RECURSE SRC "src/*.cc")

add_executable(vulkan_main)
target_sources(vulkan_main PRIVATE ${INC} ${SRC})
target_link_libraries(vulkan_main PRIVATE
    vulkan_copts_common
    vulkan_vulkan_wrappers
    vulkan_glfw_wrappers
    vulkan_common
    fmt
    glfw
    GSL
    vulkan)

configure_lto(vulkan_main)
configure_tidy(vulkan_main)
