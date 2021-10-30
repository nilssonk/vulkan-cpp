#ifndef VULKAN_SWAPCHAIN_WRAPPER_HH_INCLUDED_
#define VULKAN_SWAPCHAIN_WRAPPER_HH_INCLUDED_

#include "handle_wrapper.hh"

#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

struct QueueFamilyIndices;
struct SwapchainSupportDetails;

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
    getImages() const -> std::vector<VkImage> const &
    {
        return images_;
    }

    [[nodiscard]] auto
    getFormat() const -> VkFormat const &
    {
        return format_;
    }

    [[nodiscard]] auto
    getExtent() const -> VkExtent2D const &
    {
        return extent_;
    }

    [[nodiscard]] static auto
    create(SwapchainSupportDetails const & swapchain_details,
           VkDevice                        dev,
           VkSurfaceKHR                    surface,
           QueueFamilyIndices const &      indices,
           VkSwapchainKHR                  old_swapchain,
           uint32_t                        width,
           uint32_t height) -> std::optional<SwapchainWrapper>;

    static void
    closeHandle(SwapchainInstance handle);

    [[nodiscard]] auto
    get() const -> VkSwapchainKHR;
};

} // namespace vulkan

#endif // VULKAN_SWAPCHAIN_WRAPPER_HH_INCLUDED_
