#ifndef VULKAN_LOGICAL_DEVICE_WRAPPER_HH_INCLUDED_
#define VULKAN_LOGICAL_DEVICE_WRAPPER_HH_INCLUDED_

#include "gsl/span"
#include "handle_wrapper.hh"
#include "vulkan_helper_types.hh"

namespace vulkan {

struct LogicalDeviceWrapper : HandleWrapper<VkDevice, LogicalDeviceWrapper> {
    explicit LogicalDeviceWrapper(VkDevice dev);

    static auto
    errorMsg(const std::string & msg) -> void;

    static auto
    create(VkPhysicalDevice              phys_dev,
           const QueueFamilyIndices &    indices,
           gsl::span<const char * const> extensions)
        -> std::optional<LogicalDeviceWrapper>;

    static auto
    closeHandle(VkDevice handle) -> void;

    [[nodiscard]] auto
    get() const -> VkDevice;
};

} // namespace vulkan

#endif // VULKAN_LOGICAL_DEVICE_WRAPPER_HH_INCLUDED_
