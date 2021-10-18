#ifndef VULKAN_TO_STRING_HH_INCLUDED_
#define VULKAN_TO_STRING_HH_INCLUDED_

#include "fmt/core.h"
#include "vulkan_helper_types.hh"
#include "vulkan_version.hh"

#include <vulkan/vulkan.hpp>

constexpr auto
to_string(VkPhysicalDeviceType type) -> const char *
{
    switch (type) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "Other";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "Discrete GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "Virtual GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM: // Fallthrough
        default: return "Erroneous";
    }
}

inline auto
to_string(const QueueFamilyIndices & ind) -> std::string
{
    auto print = [](const char * fmt_str, auto const & opt) {
        return opt.has_value() ? fmt::format(fmt_str, *opt)
                               : fmt::format(fmt_str, "N/A");
    };

    return print("Graphics Family: {}\n", ind.graphics_family) +
           print("Compute Family: {}\n", ind.compute_family) +
           print("Transfer Family: {}\n", ind.transfer_family) +
           print("Sparse Binding Family: {}\n", ind.sparse_binding_family) +
           print("Protected Family: {}\n", ind.protected_family) +
           print("Present Family: {}\n", ind.present_family);
}

inline auto
to_string(const VkPhysicalDeviceProperties & prop) -> std::string
{
    const auto [major, minor, patch] = vk_version(prop.apiVersion);
    return fmt::format("Device Name: {}\n"
                       "Device Type: {}\n"
                       "Device ID: {}\n"
                       "Vendor ID: {}\n"
                       "API Version: {}.{}.{}\n",
                       prop.deviceName,
                       prop.deviceType,
                       prop.deviceID,
                       prop.vendorID,
                       major,
                       minor,
                       patch);
}

#endif // VULKAN_TO_STRING_HH_INCLUDED_
