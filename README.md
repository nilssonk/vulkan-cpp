# vulkan-cpp

This is a modern C++17 version of the example code from https://vulkan-tutorial.com

![Clang Build and Lint](https://github.com/nilssonk/vulkan-cpp/actions/workflows/clang-build.yml/badge.svg)
![GCC Build](https://github.com/nilssonk/vulkan-cpp/actions/workflows/gcc-build.yml/badge.svg)

## Dependencies

There are a few dependencies required to build this example:

### Bundled/Fetched by CMake

-   fmt (https://github.com/fmtlib/fmt)
-   GSL (https://github.com/microsoft/GSL)

### Not included

-   GLFW3 (https://github.com/glfw/glfw)
-   Vulkan headers (https://github.com/KhronosGroup/Vulkan-Headers)
-   Vulkan validation layers (https://github.com/KhronosGroup/Vulkan-ValidationLayers)
-   The glslc compiler from shaderc (https://github.com/google/shaderc)

In order to run this example successfully, a working Vulkan runtime must be present and configured on your system.

## Building

The recommended way of building this project is as follows:

1. Grab the source code

```
git clone https://github.com/nilssonk/vulkan-cpp
cd vulkan-cpp
```

2. Create and enter the build directory

```
mkdir -p build
cd build
```

3. Run CMake to configure the project

```
# Debug build with clang-tidy static analysis enabled
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DVULKAN_CLANG_TIDY=ON ../
# Release build with LTO enabled
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DVULKAN_CLANG_TIDY=OFF -DVULKAN_LTO=ON ../
```

4. Build using your configured build system

```
ninja
```

## Running the example

The example should be run in main build directory so that it can find the compiled shader files:

```
# In the build directory
main/vulkan_main
```
