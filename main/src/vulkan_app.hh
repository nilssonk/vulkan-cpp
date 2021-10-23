#ifndef VULKAN_APP_HH_INCLUDED_
#define VULKAN_APP_HH_INCLUDED_

#include "glfw_window_wrapper.hh"
#include "main_aliases.hh"
#include "to_array.hh"
#include "vulkan_helper_types.hh"
#include "vulkan_image_view_wrapper.hh"
#include "vulkan_instance_wrapper.hh"
#include "vulkan_logical_device_wrapper.hh"
#include "vulkan_surface_wrapper.hh"
#include "vulkan_swapchain_wrapper.hh"

#include <optional>
#include <unordered_map>
#include <vector>

class VulkanApp {
    static constexpr auto kRequiredExtensions =
        to_array({VK_KHR_SWAPCHAIN_EXTENSION_NAME});

    glfw::WindowWrapper                         window_;
    vulkan::InstanceWrapper                     vulkan_;
    vulkan::SurfaceWrapper                      surface_;
    QueueFamilyIndices                          indices_{};
    std::optional<vulkan::LogicalDeviceWrapper> dev_{};
    std::optional<vulkan::SwapchainWrapper>     swapchain_{};
    PropertyMap                                 properties_{};
    FeatureMap                                  features_{};
    std::vector<vulkan::ImageViewWrapper>       image_views_{};

    auto
    init() -> bool;

    auto
    loop() -> bool;

    auto
    cleanup() -> void;

    auto
    createSwapchain(VkPhysicalDevice phys_dev) -> bool;

    auto
    createImageViews() -> bool;

    auto
    loadDeviceInfo(std::vector<VkPhysicalDevice> const & devices) -> void;

public:
    VulkanApp(glfw::WindowWrapper &&     window,
              vulkan::InstanceWrapper && vulkan,
              vulkan::SurfaceWrapper &&  surface);

    auto
    run() -> int;
};

#endif // VULKAN_APP_HH_INCLUDED_
