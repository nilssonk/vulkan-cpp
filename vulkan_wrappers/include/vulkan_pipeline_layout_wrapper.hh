#ifndef VULKAN_PIPELINE_LAYOUT_WRAPPER_HH_INCLUDED_
#define VULKAN_PIPELINE_LAYOUT_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.h>

namespace vulkan {

struct PipelineLayoutHandle {
    VkPipelineLayout layout;
    VkDevice         dev;
};

struct PipelineLayoutWrapper final
    : HandleWrapper<PipelineLayoutHandle, PipelineLayoutWrapper> {
    [[nodiscard]] static auto
    create(VkDevice dev, VkPipelineLayoutCreateInfo const & create_info)
        -> std::optional<PipelineLayoutWrapper>;

    static void
    closeHandle(PipelineLayoutHandle handle);

    [[nodiscard]] auto
    get() const -> VkPipelineLayout;
};

} // namespace vulkan

#endif // VULKAN_PIPELINE_LAYOUT_WRAPPER_HH_INCLUDED_
