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
public:
    struct WindowStage {
        glfw::WindowWrapper     window;
        vulkan::InstanceWrapper instance;
        vulkan::SurfaceWrapper  surface;
    };

private:
    struct DeviceStage;
    struct SwapchainStage;
    struct ImageViewStage;
    struct PipelineStage;
    struct FramebufferStage;
    struct CompletedStage;

    static constexpr auto kRequiredExtensions =
        to_array({VK_KHR_SWAPCHAIN_EXTENSION_NAME});

    [[nodiscard]] static auto
    loop(CompletedStage & completed_stage) -> bool;

    void
    cleanup();

    [[nodiscard]] static auto
    loadDeviceInfo(std::vector<VkPhysicalDevice> const & devices)
        -> std::pair<PropertyMap, FeatureMap>;

    [[nodiscard]] static auto
    initDevices(WindowStage && window_stage) -> std::optional<DeviceStage>;

    [[nodiscard]] static auto
    createSwapchain(DeviceStage && device_stage)
        -> std::optional<SwapchainStage>;

    [[nodiscard]] static auto
    createImageViews(SwapchainStage && swapchain_stage)
        -> std::optional<ImageViewStage>;

    [[nodiscard]] static auto
    createPipeline(ImageViewStage && image_view_stage)
        -> std::optional<PipelineStage>;

    [[nodiscard]] static auto
    createFramebuffers(PipelineStage && pipeline_stage)
        -> std::optional<FramebufferStage>;

    [[nodiscard]] static auto
    init(WindowStage && window_stage) -> std::optional<CompletedStage>;

public:
    [[nodiscard]] auto
    run(WindowStage && window_stage) -> int;
};

#endif // VULKAN_APP_HH_INCLUDED_
