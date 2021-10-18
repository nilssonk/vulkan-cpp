#include "vulkan_image_view_wrapper.hh"

#include "fmt/core.h"

namespace vulkan {

auto
ImageViewWrapper::create(VkDevice dev, VkImage image, VkFormat format)
    -> std::optional<ImageViewWrapper>
{
    VkImageViewCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = image;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    auto & sub = create_info.subresourceRange;
    sub.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    sub.baseMipLevel = 0;
    sub.levelCount = 1;
    sub.baseArrayLayer = 0;
    sub.layerCount = 1;

    ImageViewHandle handle{};
    handle.dev = dev;
    if (vkCreateImageView(dev, &create_info, nullptr, &handle.image) !=
        VK_SUCCESS) {
        return std::nullopt;
    }

    fmt::print("Created VkImageView {}\n", static_cast<void *>(handle.image));

    auto result = std::make_optional<ImageViewWrapper>();
    (*result).setHandle(handle);

    return result;
}

auto
ImageViewWrapper::closeHandle(ImageViewHandle handle) -> void
{
    if (handle.image != nullptr && handle.dev != nullptr) {
        fmt::print("Destroying VkImageView {}\n",
                   static_cast<void *>(handle.image));
        vkDestroyImageView(handle.dev, handle.image, nullptr);
    }
}

} // namespace vulkan