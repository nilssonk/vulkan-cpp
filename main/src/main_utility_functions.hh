#ifndef MAIN_UTILITY_FUNCTIONS_HH_INCLUDED_
#define MAIN_UTILITY_FUNCTIONS_HH_INCLUDED_

#include "vulkan_helper_types.hh"

#include <optional>

inline auto
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

template<std::size_t N>
inline auto
check_extension_support(VkPhysicalDevice                    dev,
                        const std::array<const char *, N> & requested_extensions)
    -> bool
{
    uint32_t extension_count{};
    vkEnumerateDeviceExtensionProperties(
        dev, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(
        dev, nullptr, &extension_count, supported_extensions.data());

    return std::all_of(requested_extensions.cbegin(),
                       requested_extensions.cend(),
                       [&supported_extensions](const auto * requested) {
                           return std::any_of(
                               supported_extensions.cbegin(),
                               supported_extensions.cend(),
                               [requested](const auto & supported) {
                                   return strcmp(requested,
                                                 supported.extensionName) == 0;
                               });
                       });
}

inline auto
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
    for (const auto & fam : queue_families) {
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

inline auto
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

#endif // MAIN_UTILITY_FUNCTIONS_HH_INCLUDED_
