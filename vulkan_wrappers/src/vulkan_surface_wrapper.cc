#include "vulkan_surface_wrapper.hh"

#include "vulkan_instance_wrapper.hh"
#include "fmt/core.h"
#include "glfw_window_wrapper.hh"

#include <GLFW/glfw3.h>

namespace vulkan {

auto
SurfaceWrapper::create(InstanceWrapper & vulkan, glfw::WindowWrapper & window)
    -> std::optional<SurfaceWrapper>
{
    VkInstance instance = vulkan.get();

    VkSurfaceKHR surface{};
    if (glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) !=
        VK_SUCCESS) {
        fmt::print(stderr, "Unable to create GLFW Vulkan surface\n");
        return std::nullopt;
    }

    auto result = std::make_optional<SurfaceWrapper>();
    (*result).setHandle({surface, instance});

    fmt::print("Created VkSurface {}\n", static_cast<void *>(surface));

    return result;
}

auto
SurfaceWrapper::closeHandle(SurfaceHandle handle) -> void
{
    if (handle.instance != nullptr && handle.surface != nullptr) {
        fmt::print("Destroying VkSurface {}\n",
                   static_cast<void *>(handle.surface));
        vkDestroySurfaceKHR(handle.instance, handle.surface, nullptr);
    }
}

auto
SurfaceWrapper::get() const -> VkSurfaceKHR
{
    return getHandle().surface;
}

} // namespace vulkan
