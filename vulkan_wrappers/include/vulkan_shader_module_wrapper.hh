#ifndef VULKAN_SHADER_MODULE_WRAPPER_HH_INCLUDED_
#define VULKAN_SHADER_MODULE_WRAPPER_HH_INCLUDED_

#include "gsl/span"
#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.hpp>

namespace vulkan {

struct ShaderModuleHandle {
    VkDevice       device;
    VkShaderModule module;
};

class ShaderModuleWrapper final
    : HandleWrapper<ShaderModuleHandle, ShaderModuleWrapper> {
public:
    static auto
    create(VkDevice device, gsl::span<char const> data)
        -> std::optional<ShaderModuleWrapper>;

    static void
    closeHandle(ShaderModuleHandle handle);
};

} // namespace vulkan

#endif // VULKAN_SHADER_MODULE_WRAPPER_HH_INCLUDED_
