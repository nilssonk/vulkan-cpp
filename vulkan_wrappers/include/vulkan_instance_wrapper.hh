#ifndef VULKAN_INSTANCE_WRAPPER_HH_INCLUDED_
#define VULKAN_INSTANCE_WRAPPER_HH_INCLUDED_

#include "gsl/span"
#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.hpp>

namespace vulkan {

struct InstanceWrapper final : HandleWrapper<VkInstance, InstanceWrapper> {
    static auto
    create(gsl::span<const char * const> enabled_layers)
        -> std::optional<InstanceWrapper>;

    static auto
    closeHandle(VkInstance handle) -> void;

    [[nodiscard]] auto
    get() const -> VkInstance;
};

} // namespace vulkan

#endif // VULKAN_INSTANCE_WRAPPER_HH_INCLUDED_
