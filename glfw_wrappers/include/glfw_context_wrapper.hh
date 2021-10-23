#ifndef GLFW_CONTEXT_WRAPPER_HH_INCLUDED_
#define GLFW_CONTEXT_WRAPPER_HH_INCLUDED_

#include "gsl/span"

#include <optional>

namespace glfw {

struct WindowWrapper;

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
    getInstanceExtensions() const -> gsl::span<char const *>;
};

} // namespace glfw

#endif // GLFW_CONTEXT_WRAPPER_HH_INCLUDED_
