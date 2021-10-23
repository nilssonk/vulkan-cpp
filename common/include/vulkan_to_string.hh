#ifndef VULKAN_TO_STRING_HH_INCLUDED_
#define VULKAN_TO_STRING_HH_INCLUDED_

#include <vulkan/vulkan.hpp>

struct QueueFamilyIndices;

constexpr auto
to_string(VkPhysicalDeviceType type) -> char const *;

auto
to_string(QueueFamilyIndices const & ind) -> std::string;

auto
to_string(VkPhysicalDeviceProperties const & prop) -> std::string;

#endif // VULKAN_TO_STRING_HH_INCLUDED_
