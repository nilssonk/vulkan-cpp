#ifndef VULKAN_RENDER_PASS_WRAPPER_HH_INCLUDED_
#define VULKAN_RENDER_PASS_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.hpp>

namespace vulkan {

struct RenderPassHandle {
    VkDevice     device;
    VkRenderPass render_pass;
};

struct RenderPassWrapper final
    : HandleWrapper<RenderPassHandle, RenderPassWrapper> {
    static auto
    create(VkDevice                                     dev,
           std::vector<VkAttachmentDescription> const & attachments,
           std::vector<VkSubpassDescription> const &    subpasses)
        -> std::optional<RenderPassWrapper>;

    static void
    closeHandle(RenderPassHandle handle);

    [[nodiscard]] auto
    get() const -> VkRenderPass;
};

} // namespace vulkan

#endif // VULKAN_RENDER_PASS_WRAPPER_HH_INCLUDED_