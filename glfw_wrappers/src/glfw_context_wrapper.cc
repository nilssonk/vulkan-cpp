#include "glfw_context_wrapper.hh"

#include "fmt/core.h"

#include <memory>
#include <optional>

namespace glfw {

ContextWrapper::ContextWrapper()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

ContextWrapper::~ContextWrapper()
{
    fmt::print("Destroying ContextWrapper {}\n", static_cast<void *>(this));
    glfwTerminate();
}

auto
ContextWrapper::get() -> ContextWrapper *
{
    static std::unique_ptr<ContextWrapper> instance{};

    if (!instance) {
        if (glfwInit() == GLFW_TRUE) {
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            instance.reset(new ContextWrapper());
            fmt::print("Created ContextWrapper {}\n",
                       static_cast<void *>(instance.get()));
        }
    }

    return instance.get();
}

auto
ContextWrapper::createWindow() const -> std::optional<WindowWrapper>
{
    (void)this;

    constexpr auto window_width = 640;
    constexpr auto window_height = 480;

    auto * window = glfwCreateWindow(
        window_width, window_height, "VulkanTest", nullptr, nullptr);

    if (window == nullptr) {
        return std::nullopt;
    }

    fmt::print("Created GLFWwindow {}\n", static_cast<void *>(window));

    auto result = std::make_optional<WindowWrapper>();
    (*result).setHandle(window);

    return result;
}

auto
ContextWrapper::getInstanceExtensions() const -> gsl::span<const char *>
{
    (void)this;

    uint32_t      extension_count{};
    const char ** extensions =
        glfwGetRequiredInstanceExtensions(&extension_count);
    if (extensions == nullptr) {
        return {};
    }

    return {extensions, extension_count};
}

} // namespace glfw