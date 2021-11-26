#include "vulkan_app.hh"

#include "fmt/core.h"
#include "functional_do.hh"
#include "main_utility_functions.hh"
#include "monad_impl_optional.hh"
#include "vulkan_image_view_wrapper.hh"
#include "vulkan_logical_device_wrapper.hh"
#include "vulkan_pipeline_wrapper.hh"
#include "vulkan_shader_module_wrapper.hh"
#include "vulkan_swapchain_wrapper.hh"
#include "vulkan_to_string.hh"

#include <GLFW/glfw3.h>
#include <map>
#include <vulkan/vulkan.hpp>

struct VulkanApp::DeviceStage {
    VulkanApp::WindowStage       window_stage;
    VkPhysicalDevice             phys_dev;
    QueueFamilyIndices           indices;
    vulkan::LogicalDeviceWrapper device;
};

struct VulkanApp::SwapchainStage {
    DeviceStage              device_stage;
    vulkan::SwapchainWrapper swapchain;
};

struct VulkanApp::ImageViewStage {
    SwapchainStage                        swapchain_stage;
    std::vector<vulkan::ImageViewWrapper> image_views;
};

struct VulkanApp::PipelineStage {
    ImageViewStage          image_view_stage;
    VkQueue                 graphics_queue;
    VkQueue                 present_queue;
    vulkan::PipelineWrapper pipeline;
};

struct VulkanApp::CompletedStage {
    PipelineStage pipeline_stage;
};

auto
VulkanApp::loadDeviceInfo(std::vector<VkPhysicalDevice> const & devices)
    -> std::pair<PropertyMap, FeatureMap>
{
    int device_number{1};

    auto        result = std::pair<PropertyMap, FeatureMap>();
    auto &      properties = result.first;
    auto &      features = result.second;
    std::string prop_str{"--------------- Device properties ---------------\n"};
    for (VkPhysicalDevice dev : devices) {
        // Properties
        {
            auto const [it, success] = properties.try_emplace(dev);
            assert(success);

            auto & prop = it->second;
            vkGetPhysicalDeviceProperties(dev, &prop);

            // Print device info
            prop_str += fmt::format("{}.\n", device_number++) + to_string(prop);
        }
        // Features
        {
            auto [it, success] = features.try_emplace(dev);
            assert(success);
            vkGetPhysicalDeviceFeatures(dev, &it->second);
        }
    }
    prop_str += "-------------------------------------------------\n";
    fmt::print(prop_str);

    return result;
}

auto
VulkanApp::initDevices(WindowStage && window_stage)
    -> std::optional<DeviceStage>
{
    std::vector<VkPhysicalDevice> const devices =
        get_physical_devices(window_stage.instance.get());

    if (devices.empty()) {
        fmt::print(stderr, "Vulkan: No available physical devices.\n");
        return std::nullopt;
    }

    auto [properties, features] = loadDeviceInfo(devices);

    VkPhysicalDevice phys_dev =
        choose_best_device(devices, properties, features);

    if (!check_extension_support(phys_dev, kRequiredExtensions)) {
        fmt::print(stderr, "Vulkan: Required extensions not supported.\n");
        return std::nullopt;
    }
    auto indices =
        get_queue_family_indices(phys_dev, window_stage.surface.get());
    fmt::print(to_string(indices));

    // Create logical device
    auto dev = vulkan::LogicalDeviceWrapper::create(
        phys_dev, indices, gsl::span{kRequiredExtensions});
    if (!dev.has_value()) {
        fmt::print(stderr, "Vulkan: Logical device creation failed.\n");
        return std::nullopt;
    }

    return DeviceStage{
        std::move(window_stage), phys_dev, std::move(indices), std::move(*dev)};
}

auto
VulkanApp::createSwapchain(DeviceStage && device_stage)
    -> std::optional<SwapchainStage>
{
    int width{};
    int height{};
    glfwGetFramebufferSize(
        device_stage.window_stage.window.get(), &width, &height);

    auto * const surface = device_stage.window_stage.surface.get();

    auto const maybe_swapchain_support_details =
        query_swapchain_support(device_stage.phys_dev, surface);
    if (!maybe_swapchain_support_details.has_value()) {
        fmt::print(stderr,
                   "Vulkan: Unable to query swap chain support details.\n");
        return std::nullopt;
    }

    auto swapchain =
        vulkan::SwapchainWrapper::create(*maybe_swapchain_support_details,
                                         device_stage.device.get(),
                                         surface,
                                         device_stage.indices,
                                         VK_NULL_HANDLE,
                                         width,
                                         height);
    if (!swapchain.has_value()) {
        fmt::print(stderr, "Vulkan: Swap chain creation failed.\n");
        return std::nullopt;
    }

    return SwapchainStage{std::move(device_stage), std::move(*swapchain)};
}

auto
VulkanApp::createImageViews(SwapchainStage && swapchain_stage)
    -> std::optional<ImageViewStage>
{
    auto const & images = swapchain_stage.swapchain.getImages();
    VkFormat     format = swapchain_stage.swapchain.getFormat();

    std::vector<vulkan::ImageViewWrapper> image_views;
    image_views.reserve(images.size());
    for (VkImage image : images) {
        auto maybe_iv = vulkan::ImageViewWrapper::create(
            swapchain_stage.device_stage.device.get(), image, format);
        if (!maybe_iv.has_value()) {
            return std::nullopt;
        }
        image_views.emplace_back(std::move(*maybe_iv));
    }

    return ImageViewStage{std::move(swapchain_stage), std::move(image_views)};
}

auto
VulkanApp::createPipeline(ImageViewStage && image_view_stage)
    -> std::optional<PipelineStage>
{
    auto & s_swapchain = image_view_stage.swapchain_stage;
    auto & s_device = s_swapchain.device_stage;

    auto & indices = s_device.indices;
    auto * dev = s_device.device.get();

    VkQueue graphics_queue{};
    vkGetDeviceQueue(dev, *indices.graphics_family, 0, &graphics_queue);

    VkQueue present_queue{};
    vkGetDeviceQueue(dev, *indices.present_family, 0, &present_queue);

    (void)graphics_queue;
    (void)present_queue;
    auto const path = std::filesystem::absolute("shaders");
    auto const shader_map = load_shaders_from_path(path);
    if (shader_map.empty()) {
        fmt::print(stderr, "Found no shaders at \"{}\"\n", path.string());
        return std::nullopt;
    }

    for (auto const & [k, _] : shader_map) {
        fmt::print("Shader: {}\n", k);
    }

    using vulkan::PipelineWrapper;
    using vulkan::ShaderModuleWrapper;
    using vulkan::ShaderType;

    auto make_shader = [dev, &shader_map](ShaderType type, auto const & name) {
        auto const it = shader_map.find(name);
        return it != shader_map.end()
                   ? ShaderModuleWrapper::create(dev, type, it->second)
                   : std::nullopt;
    };
    auto maybe_vert = make_shader(ShaderType::VERT, "simple.vert.glsl");
    auto maybe_frag = make_shader(ShaderType::FRAG, "simple.frag.glsl");

    assert(maybe_vert.has_value());
    assert(maybe_frag.has_value());

    std::vector<ShaderModuleWrapper> shaders{};
    shaders.push_back(std::move(*maybe_vert));
    shaders.push_back(std::move(*maybe_frag));

    auto const & swapchain = s_swapchain.swapchain;

    auto pipeline = vulkan::PipelineWrapper::create(
        dev, std::move(shaders), swapchain.getExtent(), swapchain.getFormat());
    if (!pipeline.has_value()) {
        return std::nullopt;
    }

    return PipelineStage{std::move(image_view_stage),
                         graphics_queue,
                         present_queue,
                         std::move(*pipeline)};
}

auto
VulkanApp::init(WindowStage && window_stage) -> std::optional<CompletedStage>
{
    glfwSwapInterval(1);

    return functional_do(
        std::make_optional(std::move(window_stage)),
        initDevices,
        createSwapchain,
        createImageViews,
        createPipeline,
        [](PipelineStage && pipeline_stage) -> std::optional<CompletedStage> {
            return CompletedStage{std::move(pipeline_stage)};
        });
}

auto
VulkanApp::loop(CompletedStage & completed_stage) -> bool
{
    auto & window = completed_stage.pipeline_stage.image_view_stage
                        .swapchain_stage.device_stage.window_stage.window;
    glfwPollEvents();
    glfwSwapBuffers(window.get());
    return (glfwWindowShouldClose(window.get()) == 0);
}

void
VulkanApp::cleanup()
{
    (void)this;
}

auto
VulkanApp::run(WindowStage && window_stage) -> int
{
    auto completed_stage = init(std::move(window_stage));
    if (!completed_stage.has_value()) {
        return -1;
    }

    bool running{};
    do {
        running = loop(*completed_stage);
    } while (running);

    cleanup();

    return 0;
}