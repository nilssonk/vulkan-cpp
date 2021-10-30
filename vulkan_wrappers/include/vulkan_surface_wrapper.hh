#ifndef VULKAN_SURFACE_WRAPPER_HH_INCLUDED_
#define VULKAN_SURFACE_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.hpp>

namespace glfw {
struct WindowWrapper;
} // namespace glfw

namespace vulkan {

struct InstanceWrapper;

struct SurfaceHandle {
    VkSurfaceKHR surface;
    VkInstance   instance;
};

struct SurfaceWrapper final : HandleWrapper<SurfaceHandle, SurfaceWrapper> {
    [[nodiscard]] static auto
    create(InstanceWrapper & vulkan, glfw::WindowWrapper & window)
        -> std::optional<SurfaceWrapper>;

    static void
    closeHandle(SurfaceHandle handle);

    [[nodiscard]] auto
    get() const -> VkSurfaceKHR;
};

} // namespace vulkan

#endif // VULKAN_SURFACE_WRAPPER_HH_INCLUDED_
