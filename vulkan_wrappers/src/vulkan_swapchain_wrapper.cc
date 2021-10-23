#include "vulkan_swapchain_wrapper.hh"

#include "fmt/core.h"
#include "to_array.hh"
#include "vulkan_helper_types.hh"

#include <limits>

namespace vulkan {

auto
SwapchainWrapper::create(SwapchainSupportDetails const & swapchain_details,
                         VkDevice                        dev,
                         VkSurfaceKHR                    surface,
                         QueueFamilyIndices const &      indices,
                         VkSwapchainKHR                  old_swapchain,
                         uint32_t                        width,
                         uint32_t height) -> std::optional<SwapchainWrapper>
{
    constexpr auto preferred_format = VK_FORMAT_B8G8R8A8_SRGB;
    constexpr auto preferred_color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    constexpr auto preferred_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;

    if (swapchain_details.formats.empty()) {
        fmt::print(stderr, "Swap chain has no supported formats.\n");
        return std::nullopt;
    }

    if (swapchain_details.present_modes.empty()) {
        fmt::print(stderr, "Swap chain has no supported present modes.\n");
        return std::nullopt;
    }

    auto const format_it =
        std::find_if(swapchain_details.formats.cbegin(),
                     swapchain_details.formats.cend(),
                     [](auto const & x) {
                         return x.format == preferred_format &&
                                x.colorSpace == preferred_color_space;
                     });

    if (format_it == swapchain_details.formats.cend()) {
        fmt::print(stderr,
                   "Swap chain does not support our preferred color format.\n");
        return std::nullopt;
    }

    bool const supports_present_mode = std::any_of(
        swapchain_details.present_modes.cbegin(),
        swapchain_details.present_modes.cend(),
        [](auto const mode) { return mode == preferred_present_mode; });

    if (!supports_present_mode) {
        fmt::print(stderr,
                   "Swap chain does not support our preferred present mode.\n");
        return std::nullopt;
    }

    auto const & caps = swapchain_details.capabilities;

    VkExtent2D const extent = std::invoke([&caps, height, width] {
        if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return caps.currentExtent;
        }

        VkExtent2D result{};
        result.width = std::clamp(
            width, caps.minImageExtent.width, caps.maxImageExtent.width);
        result.height = std::clamp(
            height, caps.minImageExtent.height, caps.maxImageExtent.height);

        return result;
    });

    fmt::print("Swap chain extent: {}x{}\n", extent.width, extent.height);

    uint32_t const image_count = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && image_count > caps.maxImageCount) {
        fmt::print(stderr, "Insufficient swap chain image count.\n");
        return std::nullopt;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = format_it->format;
    create_info.imageColorSpace = format_it->colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto const queue_family_indices =
        to_array({*indices.graphics_family, *indices.present_family});
    if (queue_family_indices[0] != queue_family_indices[1]) {
        fmt::print("Swap chain using concurrent sharing mode.\n");
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = queue_family_indices.size();
        create_info.pQueueFamilyIndices = queue_family_indices.data();
    } else {
        fmt::print("Swap chain using exclusive sharing mode.\n");
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform = caps.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = old_swapchain;

    if (old_swapchain != VK_NULL_HANDLE) {
        fmt::print("Recreating swap chain using old handle.\n");
    }

    VkSwapchainKHR swapchain{};
    if (vkCreateSwapchainKHR(dev, &create_info, nullptr, &swapchain) !=
        VK_SUCCESS) {
        fmt::print(stderr, "Failed to create swap chain.\n");
        return std::nullopt;
    }

    auto   result = std::make_optional<SwapchainWrapper>();
    auto & wrapper = *result;
    wrapper.setHandle({swapchain, dev});

    uint32_t new_image_count{};
    vkGetSwapchainImagesKHR(dev, swapchain, &new_image_count, nullptr);
    if (new_image_count != image_count) {
        fmt::print("Warning: Image count differs after swapchain creation"
                   " ({} != {}).\n",
                   new_image_count,
                   image_count);
    }

    wrapper.images_.resize(new_image_count);
    vkGetSwapchainImagesKHR(
        dev, swapchain, &new_image_count, wrapper.images_.data());
    wrapper.format_ = format_it->format;
    wrapper.extent_ = extent;

    return result;
}

auto
SwapchainWrapper::closeHandle(SwapchainInstance handle) -> void
{
    if (handle.swapchain != nullptr && handle.device != nullptr) {
        fmt::print("Destroying SwapchainKHR {}\n",
                   static_cast<void *>(handle.swapchain));
        vkDestroySwapchainKHR(handle.device, handle.swapchain, nullptr);
    }
}

auto
SwapchainWrapper::get() const -> VkSwapchainKHR
{
    return getHandle().swapchain;
}

} // namespace vulkan
