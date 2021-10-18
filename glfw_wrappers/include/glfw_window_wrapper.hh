#ifndef GLFW_WINDOW_WRAPPER_HH_INCLUDED_
#define GLFW_WINDOW_WRAPPER_HH_INCLUDED_

#include "GLFW/glfw3.h"
#include "handle_wrapper.hh"

namespace glfw {

struct WindowWrapper final : HandleWrapper<GLFWwindow *, WindowWrapper> {
    friend class ContextWrapper;

    static auto
    closeHandle(GLFWwindow * handle) -> void;

    [[nodiscard]] auto
    get() const -> GLFWwindow *;
};

} // namespace glfw

#endif // GLFW_WINDOW_WRAPPER_HH_INCLUDED_
