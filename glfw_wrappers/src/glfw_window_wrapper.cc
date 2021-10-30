#include "glfw_window_wrapper.hh"

#include "fmt/core.h"

#include <GLFW/glfw3.h>

namespace glfw {

void
WindowWrapper::closeHandle(GLFWwindow * handle)
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