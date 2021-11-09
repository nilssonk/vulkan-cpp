#include "vulkan_render_pass_wrapper.hh"

#include "fmt/core.h"

namespace vulkan {

auto
RenderPassWrapper::create(
    VkDevice                                     dev,
    std::vector<VkAttachmentDescription> const & attachments,
    std::vector<VkSubpassDescription> const &    subpasses)
    -> std::optional<RenderPassWrapper>
{
    RenderPassHandle handle{};
    handle.dev = dev;

    VkRenderPassCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount = attachments.size();
    create_info.pAttachments = attachments.data();
    create_info.subpassCount = subpasses.size();
    create_info.pSubpasses = subpasses.data();
    if (vkCreateRenderPass(dev, &create_info, nullptr, &handle.render_pass) !=
        VK_SUCCESS) {
        return std::nullopt;
    }

    auto result = std::make_optional<RenderPassWrapper>();
    (*result).setHandle(handle);

    fmt::print("Created VkRenderPass {}\n",
               static_cast<void *>(handle.render_pass));

    return result;
}

void
RenderPassWrapper::closeHandle(RenderPassHandle handle)
{
    if (handle.dev != nullptr && handle.render_pass != nullptr) {
        fmt::print("Destroying VkRenderPass {}\n",
                   static_cast<void *>(handle.render_pass));
        vkDestroyRenderPass(handle.dev, handle.render_pass, nullptr);
    }
}

auto
RenderPassWrapper::get() const -> VkRenderPass
{
    return getHandle().render_pass;
}

} // namespace vulkan