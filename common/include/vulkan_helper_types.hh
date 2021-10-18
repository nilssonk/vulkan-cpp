#ifndef VULKAN_HELPER_TYPES_HH_INCLUDED_
#define VULKAN_HELPER_TYPES_HH_INCLUDED_

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>

using PropertyMap =
    std::unordered_map<VkPhysicalDevice, VkPhysicalDeviceProperties>;
using FeatureMap =
    std::unordered_map<VkPhysicalDevice, VkPhysicalDeviceFeatures>;

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> compute_family;
    std::optional<uint32_t> transfer_family;
    std::optional<uint32_t> sparse_binding_family;
    std::optional<uint32_t> protected_family;
    std::optional<uint32_t> present_family;
};

#endif // VULKAN_HELPER_TYPES_HH_INCLUDED_
