#include "vulkan_instance_wrapper.hh"

#include "fmt/core.h"
#include "glfw_context_wrapper.hh"

#include <cassert>

namespace vulkan {

namespace {

auto
layer_support_check(gsl::span<const char * const> enabled_layers)
    -> std::error_code
{
    uint32_t layer_count{};
    if (vkEnumerateInstanceLayerProperties(&layer_count, nullptr) !=
        VK_SUCCESS) {
        fmt::print(stderr, "Unable to get layer property count\n");
        return std::make_error_code(std::errc::io_error);
    }

    std::vector<VkLayerProperties> available_layers(layer_count);
    if (vkEnumerateInstanceLayerProperties(
            &layer_count, available_layers.data()) != VK_SUCCESS) {
        fmt::print(stderr, "Unable to enumerate layer properties\n");
        return std::make_error_code(std::errc::io_error);
    }

    fmt::print("Validating layers...\n");
    for (const char * enabled : enabled_layers) {
        assert(enabled != nullptr);
        fmt::print("\"{}\" - ", enabled);
        auto it = std::find_if(available_layers.cbegin(),
                               available_layers.cend(),
                               [enabled](const VkLayerProperties & p) {
                                   return strcmp(enabled, p.layerName) == 0;
                               });
        if (it == available_layers.cend()) {
            fmt::print("UNSUPPORTED\n");
            return std::make_error_code(std::errc::function_not_supported);
        }

        fmt::print("supported\n");
    }

    return {};
}

} // namespace

auto
InstanceWrapper::create(gsl::span<const char * const> enabled_layers)
    -> std::optional<InstanceWrapper>
{
    if (layer_support_check(enabled_layers)) {
        return std::nullopt;
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.apiVersion = VK_API_VERSION_1_0;

    const auto * const glfw = glfw::ContextWrapper::get();
    if (glfw == nullptr) {
        return std::nullopt;
    }

    const auto extensions = glfw->getInstanceExtensions();

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    create_info.enabledExtensionCount = extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();

    create_info.enabledLayerCount = enabled_layers.size();
    create_info.ppEnabledLayerNames = enabled_layers.data();

    VkInstance instance{};
    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
        return std::nullopt;
    }

    fmt::print("Created VkInstance {}\n", static_cast<void *>(instance));

    auto result = std::make_optional<InstanceWrapper>();
    (*result).setHandle(instance);

    return result;
}

auto
InstanceWrapper::closeHandle(VkInstance handle) -> void
{
    if (handle != nullptr) {
        fmt::print("Destroying VkInstance {}\n", static_cast<void *>(handle));
        vkDestroyInstance(handle, nullptr);
    }
}

[[nodiscard]] auto
InstanceWrapper::get() const -> VkInstance
{
    return getHandle();
}

} // namespace vulkan
