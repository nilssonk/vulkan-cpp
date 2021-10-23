project(vulkan_common VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE INC "include/*.hh")
file(GLOB_RECURSE SRC "src/*.cc")

add_library(vulkan_common)
target_sources(vulkan_common PRIVATE ${INC} ${SRC})
target_include_directories(vulkan_common PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(vulkan_common
    PUBLIC
    vulkan
    PRIVATE
    vulkan_copts_common
    fmt)

configure_lto(vulkan_common)
configure_tidy(vulkan_common)

