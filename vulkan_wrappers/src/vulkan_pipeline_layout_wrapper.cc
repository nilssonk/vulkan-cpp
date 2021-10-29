#include "vulkan_pipeline_layout_wrapper.hh"

#include "fmt/core.h"

namespace vulkan {

auto
PipelineLayoutWrapper::create(VkDevice                           dev,
                              VkPipelineLayoutCreateInfo const & create_info)
    -> std::optional<PipelineLayoutWrapper>
{
    PipelineLayoutHandle handle{};
    handle.device = dev;
    if (vkCreatePipelineLayout(dev, &create_info, nullptr, &handle.layout) !=
        VK_SUCCESS) {
        return std::nullopt;
    }

    auto result = std::make_optional<PipelineLayoutWrapper>();
    (*result).setHandle(handle);

    fmt::print("Created VkPipelineLayout {}\n",
               static_cast<void *>(handle.layout));

    return result;
}

void
PipelineLayoutWrapper::closeHandle(PipelineLayoutHandle handle)
{
    if (handle.device != nullptr && handle.layout != nullptr) {
        fmt::print("Destroying VkPipelineLayout {}\n",
                   static_cast<void *>(handle.layout));
        vkDestroyPipelineLayout(handle.device, handle.layout, nullptr);
    }
}

auto
PipelineLayoutWrapper::get() const -> VkPipelineLayout
{
    return getHandle().layout;
}

} // namespace vulkan