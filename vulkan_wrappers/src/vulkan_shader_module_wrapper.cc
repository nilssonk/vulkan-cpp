#include "vulkan_shader_module_wrapper.hh"

#include "fmt/core.h"

#include <limits>
#include <type_traits>

namespace vulkan {

template<typename ToType, typename FromType>
auto
make_aliasing_buffer(gsl::span<FromType> data)
{
    constexpr auto from_size =
        std::numeric_limits<FromType>::digits +
        static_cast<int>(std::numeric_limits<FromType>::is_signed);
    constexpr auto to_size =
        std::numeric_limits<ToType>::digits +
        static_cast<int>(std::numeric_limits<ToType>::is_signed);
    constexpr auto entry_multiple = to_size / from_size;
    static_assert(from_size <= to_size);

    std::vector<uint32_t> aliasing_buffer(data.size() / entry_multiple);
    std::memcpy(aliasing_buffer.data(), data.data(), data.size());

    return aliasing_buffer;
}

auto
ShaderModuleWrapper::create(VkDevice              device,
                            ShaderType            type,
                            gsl::span<char const> data)
    -> std::optional<ShaderModuleWrapper>
{
    // Prevent UB from aliasing violation by copying to a temporary buffer
    auto const aliasing_buffer = make_aliasing_buffer<uint32_t>(data);

    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = data.size();
    create_info.pCode = aliasing_buffer.data();

    ShaderModuleHandle handle{};
    handle.dev = device;
    handle.type = type;
    if (vkCreateShaderModule(device, &create_info, nullptr, &handle.module) !=
        VK_SUCCESS) {
        return std::nullopt;
    }

    auto result = std::make_optional<ShaderModuleWrapper>();
    (*result).setHandle(handle);

    fmt::print("Created VkShaderModule {}\n",
               static_cast<void *>(handle.module));

    return result;
}

void
ShaderModuleWrapper::closeHandle(ShaderModuleHandle handle)
{
    if (handle.dev != nullptr && handle.module != nullptr) {
        fmt::print("Destroying VkShaderModule {}\n",
                   static_cast<void *>(handle.module));
        vkDestroyShaderModule(handle.dev, handle.module, nullptr);
    }
}

auto
ShaderModuleWrapper::module() const -> VkShaderModule
{
    return getHandle().module;
}

auto
ShaderModuleWrapper::type() const -> ShaderType
{
    return getHandle().type;
}

} // namespace vulkan