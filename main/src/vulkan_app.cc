#include "vulkan_app.hh"

#include "fmt/core.h"
#include "main_utility_functions.hh"
#include "vulkan_to_string.hh"

#include <GLFW/glfw3.h>
#include <map>
#include <vulkan/vulkan.hpp>

VulkanApp::VulkanApp(glfw::WindowWrapper &&     window,
                     vulkan::InstanceWrapper && vulkan,
                     vulkan::SurfaceWrapper &&  surface)
    : window_{std::move(window)},
      vulkan_{std::move(vulkan)},
      surface_{std::move(surface)}
{
}

void
VulkanApp::loadDeviceInfo(std::vector<VkPhysicalDevice> const & devices)
{
    int         device_number{1};
    std::string prop_str{"--------------- Device properties ---------------\n"};
    for (VkPhysicalDevice dev : devices) {
        // Properties
        {
            auto const [it, success] = properties_.try_emplace(dev);
            assert(success);

            auto & prop = it->second;
            vkGetPhysicalDeviceProperties(dev, &prop);

            // Print device info
            prop_str += fmt::format("{}.\n", device_number++) + to_string(prop);
        }
        // Features
        {
            auto [it, success] = features_.try_emplace(dev);
            assert(success);
            vkGetPhysicalDeviceFeatures(dev, &it->second);
        }
    }
    prop_str += "-------------------------------------------------\n";
    fmt::print(prop_str);
}

auto
VulkanApp::createSwapchain(VkPhysicalDevice phys_dev) -> bool
{
    assert(dev_.has_value() && "Must have a logical device");

    VkDevice dev = (*dev_).get();

    int width{};
    int height{};
    glfwGetFramebufferSize(window_.get(), &width, &height);

    auto const maybe_swapchain_support_details =
        query_swapchain_support(phys_dev, surface_.get());
    if (!maybe_swapchain_support_details.has_value()) {
        fmt::print(stderr,
                   "Vulkan: Unable to query swap chain support details.\n");
        return false;
    }
    swapchain_ =
        vulkan::SwapchainWrapper::create(*maybe_swapchain_support_details,
                                         dev,
                                         surface_.get(),
                                         indices_,
                                         VK_NULL_HANDLE,
                                         width,
                                         height);
    if (!swapchain_.has_value()) {
        fmt::print(stderr, "Vulkan: Swap chain creation failed.\n");
        return false;
    }

    return true;
}

auto
VulkanApp::createImageViews() -> bool
{
    assert(dev_.has_value() && "Must have a logical device");

    VkDevice     dev = (*dev_).get();
    auto const & swapchain = (*swapchain_);

    auto const & images = swapchain.getImages();
    VkFormat     format = swapchain.getFormat();

    image_views_.reserve(images.size());
    for (VkImage image : images) {
        auto maybe_iv = vulkan::ImageViewWrapper::create(dev, image, format);
        if (!maybe_iv.has_value()) {
            return false;
        }
        image_views_.emplace_back(std::move(*maybe_iv));
    }

    return true;
}

auto
VulkanApp::init() -> bool
{
    glfwSwapInterval(1);

    std::vector<VkPhysicalDevice> const devices =
        get_physical_devices(vulkan_.get());

    if (devices.empty()) {
        fmt::print(stderr, "Vulkan: No available physical devices.\n");
        return false;
    }

    loadDeviceInfo(devices);

    VkPhysicalDevice phys_dev =
        choose_best_device(devices, properties_, features_);

    if (!check_extension_support(phys_dev, kRequiredExtensions)) {
        fmt::print(stderr, "Vulkan: Required extensions not supported.\n");
        return false;
    }
    indices_ = get_queue_family_indices(phys_dev, surface_.get());
    fmt::print(to_string(indices_));

    // Create logical device
    {
        dev_ = vulkan::LogicalDeviceWrapper::create(
            phys_dev, indices_, gsl::span{kRequiredExtensions});
        if (!dev_.has_value()) {
            fmt::print(stderr, "Vulkan: Logical device creation failed.\n");
            return false;
        }
    }

    if (!createSwapchain(phys_dev)) {
        return false;
    }

    if (!createImageViews()) {
        return false;
    }

    {
        VkDevice dev = (*dev_).get();

        VkQueue graphics_queue{};
        vkGetDeviceQueue(dev, *indices_.graphics_family, 0, &graphics_queue);

        VkQueue present_queue{};
        vkGetDeviceQueue(dev, *indices_.present_family, 0, &present_queue);

        (void)graphics_queue;
        (void)present_queue;
    }

    return true;
}

auto
VulkanApp::loop() -> bool
{
    glfwPollEvents();
    glfwSwapBuffers(window_.get());
    return (glfwWindowShouldClose(window_.get()) == 0);
}

void
VulkanApp::cleanup()
{
    (void)this;
}

auto
VulkanApp::run() -> int
{
    if (!init()) {
        return -1;
    }

    bool running{};
    do {
        running = loop();
    } while (running);

    cleanup();

    return 0;
}