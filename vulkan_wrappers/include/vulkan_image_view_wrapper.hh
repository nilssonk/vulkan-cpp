#ifndef VULKAN_IMAGE_VIEW_WRAPPER_HH_INCLUDED_
#define VULKAN_IMAGE_VIEW_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.hpp>

namespace vulkan {

struct ImageViewHandle {
    VkImageView image;
    VkDevice    dev;
};

struct ImageViewWrapper final
    : HandleWrapper<ImageViewHandle, ImageViewWrapper> {
    [[nodiscard]] static auto
    create(VkDevice dev, VkImage image, VkFormat format)
        -> std::optional<ImageViewWrapper>;

    static void
    closeHandle(ImageViewHandle handle);

    [[nodiscard]] auto
    get() const -> VkImageView;
};

} // namespace vulkan

#endif // VULKAN_IMAGE_VIEW_WRAPPER_HH_INCLUDED_
