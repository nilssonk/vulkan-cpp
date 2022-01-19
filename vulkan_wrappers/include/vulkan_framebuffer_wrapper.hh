#ifndef VULKAN_FRAMEBUFFER_WRAPPER_HH_INCLUDED_
#define VULKAN_FRAMEBUFFER_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace vulkan {

struct FramebufferHandle {
    VkFramebuffer framebuffer;
    VkDevice      dev;
};

struct FramebufferWrapper
    : public HandleWrapper<FramebufferHandle, FramebufferWrapper> {
    [[nodiscard]] static auto
    create(VkDevice     dev,
           VkRenderPass render_pass,
           VkImageView  view,
           VkExtent2D   extent) -> std::optional<FramebufferWrapper>;

    static void
    closeHandle(FramebufferHandle handle);

    [[nodiscard]] auto
    get() const -> VkFramebuffer;
};

} // namespace vulkan

#endif // VULKAN_FRAMEBUFFER_WRAPPER_HH_INCLUDED_
