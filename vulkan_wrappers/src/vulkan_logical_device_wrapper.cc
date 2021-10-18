#include "vulkan_logical_device_wrapper.hh"

#include "fmt/core.h"

#include <set>

namespace vulkan {

LogicalDeviceWrapper::LogicalDeviceWrapper(VkDevice dev)
{
    fmt::print("Claiming ownership of VkDevice {}\n", static_cast<void *>(dev));
    setHandle(dev);
}

auto
LogicalDeviceWrapper::errorMsg(const std::string & msg) -> void
{
    fmt::print(stderr, msg);
}

auto
LogicalDeviceWrapper::create(VkPhysicalDevice              phys_dev,
                             const QueueFamilyIndices &    indices,
                             gsl::span<const char * const> extensions)
    -> std::optional<LogicalDeviceWrapper>
{
    if (!indices.graphics_family.has_value()) {
        errorMsg("No queues supporting the graphics family available\n");
        return std::nullopt;
    }

    const std::set<uint32_t> unique_queue_families = {*indices.graphics_family,
                                                      *indices.present_family};

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos{};
    queue_create_infos.reserve(unique_queue_families.size());

    constexpr float queue_priority = 1.0F;
    for (const uint32_t queue_family : unique_queue_families) {
        queue_create_infos.emplace_back();
        auto & info = queue_create_infos.back();
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = queue_family;
        info.queueCount = 1;
        info.pQueuePriorities = &queue_priority;
    }

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pEnabledFeatures = &device_features;

    create_info.queueCreateInfoCount = queue_create_infos.size();
    create_info.pQueueCreateInfos = queue_create_infos.data();

    create_info.enabledExtensionCount = extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDevice log_dev{};
    if (vkCreateDevice(phys_dev, &create_info, nullptr, &log_dev) !=
        VK_SUCCESS) {
        errorMsg("Unable to create Vulkan logical device\n");
        return std::nullopt;
    }

    return LogicalDeviceWrapper(log_dev);
}

auto
LogicalDeviceWrapper::closeHandle(VkDevice handle) -> void
{
    if (handle != nullptr) {
        fmt::print("Destroying VkDevice {}\n", static_cast<void *>(handle));
        vkDestroyDevice(handle, nullptr);
    }
}

auto
LogicalDeviceWrapper::get() const -> VkDevice
{
    return getHandle();
}

} // namespace vulkan
