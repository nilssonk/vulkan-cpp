#ifndef VULKAN_PIPELINE_WRAPPER_HH_INCLUDED_
#define VULKAN_PIPELINE_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"
#include "vulkan_pipeline_layout_wrapper.hh"
#include "vulkan_render_pass_wrapper.hh"
#include "vulkan_shader_module_wrapper.hh"

#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace vulkan {

struct PipelineHandle {
    VkDevice   device;
    VkPipeline pipeline;
};

class PipelineWrapper final : HandleWrapper<PipelineHandle, PipelineWrapper> {
    std::vector<ShaderModuleWrapper> shaders_;
    PipelineLayoutWrapper            layout_;
    RenderPassWrapper                render_pass_;

public:
    [[nodiscard]] static auto
    create(VkDevice                            dev,
           std::vector<ShaderModuleWrapper> && shaders,
           VkExtent2D                          extent,
           VkFormat format) -> std::optional<PipelineWrapper>;

    static void
    closeHandle(PipelineHandle handle);

    [[nodiscard]] auto
    get() const -> VkPipeline;
};

} // namespace vulkan

#endif // VULKAN_PIPELINE_WRAPPER_HH_INCLUDED_
