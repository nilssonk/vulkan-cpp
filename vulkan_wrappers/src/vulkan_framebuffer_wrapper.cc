#include "vulkan_framebuffer_wrapper.hh"

#include "fmt/core.h"
#include "to_array.hh"

namespace vulkan {

[[nodiscard]] auto
FramebufferWrapper::create(VkDevice     dev,
                           VkRenderPass render_pass,
                           VkImageView  view,
                           VkExtent2D   extent)
    -> std::optional<FramebufferWrapper>
{
    auto const attachments = to_array({view});

    VkFramebufferCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    ci.renderPass = render_pass;
    ci.attachmentCount = 1;
    ci.pAttachments = attachments.data();
    ci.width = extent.width;
    ci.height = extent.height;
    ci.layers = 1;

    FramebufferHandle handle{};
    handle.dev = dev;
    if (vkCreateFramebuffer(dev, &ci, nullptr, &handle.framebuffer) !=
        VK_SUCCESS) {
        return std::nullopt;
    }

    fmt::print("Created VkFramebuffer {}\n",
               static_cast<void *>(handle.framebuffer));

    auto result = std::make_optional<FramebufferWrapper>();
    (*result).setHandle(handle);

    return result;
}

void
FramebufferWrapper::closeHandle(FramebufferHandle handle)
{
    if (handle.dev != nullptr && handle.framebuffer != nullptr) {
        fmt::print("Destroying VkFramebuffer {}\n",
                   static_cast<void *>(handle.framebuffer));
        vkDestroyFramebuffer(handle.dev, handle.framebuffer, nullptr);
    }
}

auto
FramebufferWrapper::get() const -> VkFramebuffer
{
    return getHandle().framebuffer;
}

} // namespace vulkan