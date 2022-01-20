#ifndef MAIN_UTILITY_FUNCTIONS_HH_INCLUDED_
#define MAIN_UTILITY_FUNCTIONS_HH_INCLUDED_

#include "main_aliases.hh"

#include <algorithm>
#include <array>
#include <filesystem>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

struct SwapchainSupportDetails;
struct QueueFamilyIndices;

using ShaderMap = std::unordered_map<std::string, std::vector<char>>;

[[nodiscard]] auto
load_shaders_from_path(std::filesystem::path const & path) -> ShaderMap;

[[nodiscard]] auto
rate_device(VkPhysicalDeviceFeatures const &   feat,
            VkPhysicalDeviceProperties const & prop) -> uint8_t;
[[nodiscard]] auto
choose_best_device(std::vector<VkPhysicalDevice> const & devices,
                   PropertyMap const &                   properties,
                   FeatureMap const & features) -> VkPhysicalDevice;

[[nodiscard]] auto
query_swapchain_support(VkPhysicalDevice dev, VkSurfaceKHR surface)
    -> std::optional<SwapchainSupportDetails>;

[[nodiscard]] auto
get_queue_family_indices(VkPhysicalDevice dev, VkSurfaceKHR surface)
    -> QueueFamilyIndices;

[[nodiscard]] auto
get_physical_devices(VkInstance instance) -> std::vector<VkPhysicalDevice>;

template<std::size_t N>
[[nodiscard]] inline auto
check_extension_support(VkPhysicalDevice                    dev,
                        std::array<char const *, N> const & requested_extensions)
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
                       [&supported_extensions](auto const * requested) {
                           return std::any_of(
                               supported_extensions.cbegin(),
                               supported_extensions.cend(),
                               [requested](auto const & supported) {
                                   return strcmp(requested,
                                                 supported.extensionName) == 0;
                               });
                       });
}

#endif // MAIN_UTILITY_FUNCTIONS_HH_INCLUDED_
