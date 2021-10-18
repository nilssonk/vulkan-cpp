#ifndef GLFW_CONTEXT_WRAPPER_HH_INCLUDED_
#define GLFW_CONTEXT_WRAPPER_HH_INCLUDED_

#include "glfw_window_wrapper.hh"
#include "gsl/span"

#include <optional>

namespace glfw {

class ContextWrapper {
    ContextWrapper();

public:
    ContextWrapper(const ContextWrapper &) = delete;
    ContextWrapper(ContextWrapper &&) = delete;

    ~ContextWrapper();

    auto
    operator=(const ContextWrapper &) -> ContextWrapper & = delete;
    auto
    operator=(ContextWrapper &&) -> ContextWrapper & = delete;

    [[nodiscard]] static auto
    get() -> ContextWrapper *;

    [[nodiscard]] auto
    createWindow() const -> std::optional<WindowWrapper>;

    [[nodiscard]] auto
    getInstanceExtensions() const -> gsl::span<const char *>;
};

} // namespace glfw

#endif // GLFW_CONTEXT_WRAPPER_HH_INCLUDED_
