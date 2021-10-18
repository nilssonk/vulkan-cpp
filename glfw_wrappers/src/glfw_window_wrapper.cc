#include "glfw_window_wrapper.hh"

#include "fmt/core.h"

namespace glfw {

auto
WindowWrapper::closeHandle(GLFWwindow * handle) -> void
{
    if (handle != nullptr) {
        fmt::print("Destroying GLFWwindow {}\n", static_cast<void *>(handle));
        glfwDestroyWindow(handle);
    }
}

[[nodiscard]] auto
WindowWrapper::get() const -> GLFWwindow *
{
    return getHandle();
}

} // namespace glfw