#ifndef VULKAN_INSTANCE_WRAPPER_HH_INCLUDED_
#define VULKAN_INSTANCE_WRAPPER_HH_INCLUDED_

#include "gsl/span"
#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.hpp>

namespace vulkan {

struct InstanceWrapper final : HandleWrapper<VkInstance, InstanceWrapper> {
    [[nodiscard]] static auto
    create(gsl::span<char const * const> enabled_layers)
        -> std::optional<InstanceWrapper>;

    static void
    closeHandle(VkInstance handle);

    [[nodiscard]] auto
    get() const -> VkInstance;
};

} // namespace vulkan

#endif // VULKAN_INSTANCE_WRAPPER_HH_INCLUDED_
