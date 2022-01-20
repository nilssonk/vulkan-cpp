#ifndef VULKAN_TO_STRING_HH_INCLUDED_
#define VULKAN_TO_STRING_HH_INCLUDED_

#include <string>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices;

[[nodiscard]] constexpr auto
to_string(VkPhysicalDeviceType type) -> char const *;

[[nodiscard]] auto
to_string(QueueFamilyIndices const & ind) -> std::string;

[[nodiscard]] auto
to_string(VkPhysicalDeviceProperties const & prop) -> std::string;

#endif // VULKAN_TO_STRING_HH_INCLUDED_
