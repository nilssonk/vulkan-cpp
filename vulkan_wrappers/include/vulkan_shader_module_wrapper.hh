#ifndef VULKAN_SHADER_MODULE_WRAPPER_HH_INCLUDED_
#define VULKAN_SHADER_MODULE_WRAPPER_HH_INCLUDED_

#include "gsl/span"
#include "handle_wrapper.hh"

#include <optional>
#include <vulkan/vulkan.hpp>

namespace vulkan {

enum class ShaderType
{
    FRAG,
    VERT
};

struct ShaderModuleHandle {
    VkDevice       device;
    ShaderType     type;
    VkShaderModule module;
};

struct ShaderModuleWrapper final
    : HandleWrapper<ShaderModuleHandle, ShaderModuleWrapper> {
    [[nodiscard]] static auto
    create(VkDevice device, ShaderType type, gsl::span<char const> data)
        -> std::optional<ShaderModuleWrapper>;

    static void
    closeHandle(ShaderModuleHandle handle);

    [[nodiscard]] auto
    module() const -> VkShaderModule;

    [[nodiscard]] auto
    type() const -> ShaderType;
};

} // namespace vulkan

#endif // VULKAN_SHADER_MODULE_WRAPPER_HH_INCLUDED_
