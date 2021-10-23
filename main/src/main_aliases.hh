#ifndef MAIN_ALIASES_HH_INCLUDED_
#define MAIN_ALIASES_HH_INCLUDED_

#include <unordered_map>
#include <vulkan/vulkan.hpp>

using PropertyMap =
    std::unordered_map<VkPhysicalDevice, VkPhysicalDeviceProperties>;
using FeatureMap =
    std::unordered_map<VkPhysicalDevice, VkPhysicalDeviceFeatures>;

#endif // MAIN_ALIASES_HH_INCLUDED_
