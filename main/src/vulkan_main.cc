#include "fmt/core.h"
#include "glfw_context_wrapper.hh"
#include "to_array.hh"
#include "vulkan_app.hh"
#include "vulkan_instance_wrapper.hh"

namespace {

auto
init_window() -> std::optional<glfw::WindowWrapper>
{
    auto const * const glfw = glfw::ContextWrapper::get();

    if (glfw == nullptr) {
        return std::nullopt;
    }

    return glfw->createWindow();
}

} // namespace

auto
main(int /*argc*/, char ** /*argv*/) -> int
{
    auto window = init_window();
    if (!window.has_value()) {
        fmt::print(stderr, "Failed to initialize window\n");
        return -1;
    }

    constexpr auto layers = to_array({"VK_LAYER_KHRONOS_validation"});

    auto vulkan = vulkan::InstanceWrapper::create({layers});
    if (!vulkan.has_value()) {
        fmt::print(stderr, "Failed to initialize Vulkan\n");
        return -1;
    }

    auto surface = vulkan::SurfaceWrapper::create(*vulkan, *window);
    if (!surface.has_value()) {
        fmt::print(stderr, "Failed to create Vulkan surface\n");
        return -1;
    }

    VulkanApp app{};

    return app.run(VulkanApp::WindowStage{
        std::move(*window), std::move(*vulkan), std::move(*surface)});
}