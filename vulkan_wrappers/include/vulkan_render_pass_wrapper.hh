#ifndef VULKAN_RENDER_PASS_WRAPPER_HH_INCLUDED_
#define VULKAN_RENDER_PASS_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace vulkan {

struct RenderPassHandle {
    VkRenderPass render_pass;
    VkDevice     dev;
};

struct RenderPassWrapper final
    : HandleWrapper<RenderPassHandle, RenderPassWrapper> {
    [[nodiscard]] static auto
    create(VkDevice                                     dev,
           std::vector<VkAttachmentDescription> const & attachments,
           std::vector<VkAttachmentReference> const &   references)
        -> std::optional<RenderPassWrapper>;

    static void
    closeHandle(RenderPassHandle handle);

    [[nodiscard]] auto
    get() const -> VkRenderPass;
};

} // namespace vulkan

#endif // VULKAN_RENDER_PASS_WRAPPER_HH_INCLUDED_
