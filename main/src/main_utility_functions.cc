#include "main_utility_functions.hh"

#include "fmt/core.h"
#include "vulkan_helper_types.hh"

#include <map>

auto
rate_device(VkPhysicalDeviceFeatures const &   feat,
            VkPhysicalDeviceProperties const & prop) -> uint8_t
{
    uint8_t result{};
    if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        result += 5;
    }
    result += feat.geometryShader;
    result += feat.tessellationShader;

    // @TODO: More criteria

    return result;
}

auto
choose_best_device(std::vector<VkPhysicalDevice> const & devices,
                   PropertyMap const &                   properties,
                   FeatureMap const & features) -> VkPhysicalDevice
{
    assert(!devices.empty());
    assert(!properties.empty());
    assert(!features.empty());

    std::multimap<uint8_t, VkPhysicalDevice> scores{};

    for (auto * dev : devices) {
        auto const prop_it = properties.find(dev);
        assert(prop_it != properties.end());
        auto const & prop = prop_it->second;

        auto const feat_it = features.find(dev);
        assert(feat_it != features.end());
        auto const & feat = feat_it->second;

        uint8_t const score = rate_device(feat, prop);
        fmt::print("Score: {}\n", static_cast<unsigned>(score));

        scores.emplace(score, dev);
    }

    return scores.rbegin()->second;
}

auto
query_swapchain_support(VkPhysicalDevice dev, VkSurfaceKHR surface)
    -> std::optional<SwapchainSupportDetails>
{
    SwapchainSupportDetails result{};
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            dev, surface, &result.capabilities) != VK_SUCCESS) {
        return std::nullopt;
    }

    uint32_t format_count{};
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(
            dev, surface, &format_count, nullptr) != VK_SUCCESS) {
        return std::nullopt;
    }
    if (format_count > 0) {
        result.formats.resize(format_count);
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(
                dev, surface, &format_count, result.formats.data()) !=
            VK_SUCCESS) {
            return std::nullopt;
        }
    }

    uint32_t present_mode_count{};
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(
            dev, surface, &present_mode_count, nullptr) != VK_SUCCESS) {
        return std::nullopt;
    }
    if (present_mode_count > 0) {
        result.present_modes.resize(present_mode_count);
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(
                dev,
                surface,
                &present_mode_count,
                result.present_modes.data()) != VK_SUCCESS) {
            return std::nullopt;
        }
    }

    return std::make_optional(result);
}

auto
get_queue_family_indices(VkPhysicalDevice dev, VkSurfaceKHR surface)
    -> QueueFamilyIndices
{
    assert(dev != nullptr);
    assert(surface != nullptr);

    uint32_t queue_family_count{};
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(
        dev, &queue_family_count, queue_families.data());

    QueueFamilyIndices result{};

    uint32_t i{};
    for (auto const & fam : queue_families) {
        if ((fam.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
            result.graphics_family = i;
        }
        if ((fam.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) {
            result.compute_family = i;
        }
        if ((fam.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) {
            result.transfer_family = i;
        }
        if ((fam.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) {
            result.sparse_binding_family = i;
        }
        if ((fam.queueFlags & VK_QUEUE_PROTECTED_BIT) != 0) {
            result.protected_family = i;
        }

        VkBool32 present_support{};
        if (vkGetPhysicalDeviceSurfaceSupportKHR(
                dev, i, surface, &present_support) == VK_SUCCESS &&
            present_support == VK_TRUE) {
            result.present_family = i;
        }

        ++i;
    }

    return result;
}

auto
get_physical_devices(VkInstance instance) -> std::vector<VkPhysicalDevice>
{
    uint32_t                      device_count{};
    std::vector<VkPhysicalDevice> devices{};
    if (vkEnumeratePhysicalDevices(instance, &device_count, nullptr) !=
        VK_SUCCESS) {
        return devices;
    }

    devices.resize(device_count);
    if (vkEnumeratePhysicalDevices(instance, &device_count, devices.data()) !=
        VK_SUCCESS) {
        return devices;
    }

    return devices;
}
