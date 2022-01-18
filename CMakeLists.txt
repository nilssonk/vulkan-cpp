cmake_minimum_required(VERSION 3.14)

project(vulkan VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/GLSL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
include(lto.helper)
include(test.helper)
include(with.helper)
include(tidy.helper)

option(VULKAN_CLANG_TIDY "Build with clang-tidy" ON)
option(VULKAN_TESTS "Build and add tests" ON)
option(VULKAN_COVERAGE "Build with coverage support" OFF)
option(VULKAN_LTO "Build with Link-Time Optimization" OFF)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    # Disable DeathTests
    set(ENV{EXTRA_TEST_ARGS} --gtest_filter=-*DeathTest*)
endif()

WithMsg(tests ${VULKAN_TESTS})
WithMsg(clang-tidy ${VULKAN_CLANG_TIDY})
WithMsg(coverage ${VULKAN_COVERAGE})
WithMsg(lto ${VULKAN_LTO})

if (VULKAN_CLANG_TIDY)
    include(ClangTidy)
endif()

set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --child-silent-after-fork=yes --leak-check=full --error-exitcode=5")

if (VULKAN_TESTS)
    include(CTest)
    enable_testing()
endif()

set(VULKAN_EXTRA_CFLAGS -O3 -ffast-math -fvisibility=hidden)

set_property(GLOBAL PROPERTY VULKAN_RELEASE_EXTRA_CFLAGS ${VULKAN_EXTRA_CFLAGS})

add_library(vulkan_copts_common INTERFACE)
target_compile_definitions(vulkan_copts_common INTERFACE
    VULKAN_HPP_NO_EXCEPTIONS)
target_compile_options(vulkan_copts_common INTERFACE
    -fno-exceptions
    -fno-rtti
    -pedantic
    -W
    -Wall
    -Wextra
    -Wcast-align
    -Wcast-qual
    -Wmissing-declarations
    -Wwrite-strings
    -Wundef
    -Wswitch-enum
    -Wshadow
    -Werror
    -Wstrict-aliasing=2
    $<$<C_COMPILER_ID:Clang>:-Wshadow-all>
    $<$<CXX_COMPILER_ID:Clang>:-Wshadow-all>
    $<$<STREQUAL:$<CONFIG>,Debug>:-ggdb>
    $<$<STREQUAL:$<CONFIG>,Release>:${VULKAN_EXTRA_CFLAGS}>
    $<$<BOOL:${VULKAN_COVERAGE}>:-g -fprofile-arcs -ftest-coverage>)
target_link_libraries(vulkan_copts_common
    INTERFACE
    $<$<BOOL:${VULKAN_COVERAGE}>:
        $<IF:$<STREQUAL:${CMAKE_CXX_COMPILER_ID},GNU>,
            gcov, # GCC
            --coverage # Clang
        >
    >)
#target_link_options(vulkan_copts_common INTERFACE -static-libgcc -static-libstdc++)
target_compile_definitions(vulkan_copts_common INTERFACE GLFW_INCLUDE_VULKAN)


add_subdirectory(3rdparty)

add_subdirectory(common)
add_subdirectory(glfw_wrappers)
add_subdirectory(shaders)
add_subdirectory(vulkan_wrappers)
add_subdirectory(main)
