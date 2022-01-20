#ifndef VULKAN_LOGICAL_DEVICE_WRAPPER_HH_INCLUDED_
#define VULKAN_LOGICAL_DEVICE_WRAPPER_HH_INCLUDED_

#include "gsl/span"
#include "handle_wrapper.hh"

#include <optional>
#include <string>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices;

namespace vulkan {

struct LogicalDeviceWrapper final
    : HandleWrapper<VkDevice, LogicalDeviceWrapper> {
    explicit LogicalDeviceWrapper(VkDevice dev);

    static void
    errorMsg(std::string const & msg);

    [[nodiscard]] static auto
    create(VkPhysicalDevice              phys_dev,
           QueueFamilyIndices const &    indices,
           gsl::span<char const * const> extensions)
        -> std::optional<LogicalDeviceWrapper>;

    static void
    closeHandle(VkDevice handle);

    [[nodiscard]] auto
    get() const -> VkDevice;
};

} // namespace vulkan

#endif // VULKAN_LOGICAL_DEVICE_WRAPPER_HH_INCLUDED_
