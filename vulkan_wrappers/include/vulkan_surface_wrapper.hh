#ifndef VULKAN_SURFACE_WRAPPER_HH_INCLUDED_
#define VULKAN_SURFACE_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"
#include "vulkan_instance_wrapper.hh"

namespace glfw {
struct WindowWrapper;
} // namespace glfw

namespace vulkan {

struct SurfaceHandle {
    VkSurfaceKHR surface;
    VkInstance   instance;
};

struct SurfaceWrapper : HandleWrapper<SurfaceHandle, SurfaceWrapper> {
    static auto
    create(InstanceWrapper & vulkan, glfw::WindowWrapper & window)
        -> std::optional<SurfaceWrapper>;

    static auto
    closeHandle(SurfaceHandle handle) -> void;

    [[nodiscard]] auto
    get() const -> VkSurfaceKHR;
};

} // namespace vulkan

#endif // VULKAN_SURFACE_WRAPPER_HH_INCLUDED_
