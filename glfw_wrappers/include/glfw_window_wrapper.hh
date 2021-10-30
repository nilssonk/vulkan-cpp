#ifndef GLFW_WINDOW_WRAPPER_HH_INCLUDED_
#define GLFW_WINDOW_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

class GLFWwindow;

namespace glfw {

struct WindowWrapper final : HandleWrapper<GLFWwindow *, WindowWrapper> {
    friend class ContextWrapper;

    static void
    closeHandle(GLFWwindow * handle);

    [[nodiscard]] auto
    get() const -> GLFWwindow *;
};

} // namespace glfw

#endif // GLFW_WINDOW_WRAPPER_HH_INCLUDED_
