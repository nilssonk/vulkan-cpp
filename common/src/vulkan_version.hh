#ifndef VULKAN_VERSION_HH_INCLUDED_
#define VULKAN_VERSION_HH_INCLUDED_

struct VulkanVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;

    [[nodiscard]] constexpr static auto
    from(uint32_t version) -> VulkanVersion
    {
        VulkanVersion result{};
        result.major = version >> 22;
        result.minor = (version >> 12) & 0x3ff;
        result.patch = version & 0xfff;

        return result;
    }
};

#endif // VULKAN_VERSION_HH_INCLUDED_
