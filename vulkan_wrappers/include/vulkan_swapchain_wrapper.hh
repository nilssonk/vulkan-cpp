#ifndef VULKAN_SWAPCHAIN_WRAPPER_HH_INCLUDED_
#define VULKAN_SWAPCHAIN_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"
#include "vulkan_helper_types.hh"

namespace vulkan {

struct SwapchainInstance {
    VkSwapchainKHR swapchain;
    VkDevice       device;
};

class SwapchainWrapper final
    : public HandleWrapper<SwapchainInstance, SwapchainWrapper> {
    std::vector<VkImage> images_{};
    VkFormat             format_{};
    VkExtent2D           extent_{};

public:
    [[nodiscard]] auto
    getImages() const -> const std::vector<VkImage> &
    {
        return images_;
    }

    [[nodiscard]] auto
    geFormat() const -> const VkFormat &
    {
        return format_;
    }

    [[nodiscard]] auto
    getExtent() const -> const VkExtent2D &
    {
        return extent_;
    }

    static auto
    create(const SwapchainSupportDetails & swapchain_details,
           VkDevice                        dev,
           VkSurfaceKHR                    surface,
           const QueueFamilyIndices &      indices,
           VkSwapchainKHR                  old_swapchain,
           uint32_t                        width,
           uint32_t height) -> std::optional<SwapchainWrapper>;

    static auto
    closeHandle(SwapchainInstance handle) -> void;

    [[nodiscard]] auto
    get() const -> VkSwapchainKHR;
};

} // namespace vulkan

#endif // VULKAN_SWAPCHAIN_WRAPPER_HH_INCLUDED_
