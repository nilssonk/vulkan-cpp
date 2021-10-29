#include "vulkan_pipeline_wrapper.hh"

#include "fmt/core.h"
#include "to_array.hh"

namespace vulkan {

auto
PipelineWrapper::create(VkDevice                            dev,
                        std::vector<ShaderModuleWrapper> && shaders,
                        VkExtent2D                          extent,
                        VkFormat format) -> std::optional<PipelineWrapper>
{
    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos{};
    shader_stage_create_infos.reserve(shaders.size());
    for (auto const & s : shaders) {
        auto & create_info = shader_stage_create_infos.emplace_back();

        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        switch (s.type()) {
            case ShaderType::FRAG:
                create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                break;
            case ShaderType::VERT:
                create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
                break;
            default:
                fmt::print(stderr, "Unhandled shader type: {}", s.type());
                return std::nullopt;
        }
        create_info.module = s.module();
        create_info.pName = "main";
    }

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
    {
        auto & vi = vertex_input_state_create_info;
        vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vi.vertexBindingDescriptionCount = 0;
        vi.pVertexBindingDescriptions = nullptr;
        vi.vertexAttributeDescriptionCount = 0;
        vi.pVertexAttributeDescriptions = nullptr;
    }

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{};
    {
        auto & ia = input_assembly_state_create_info;
        ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        ia.primitiveRestartEnable = VK_FALSE;
    }

    VkViewport viewport{};
    viewport.x = 0.0F;
    viewport.y = 0.0F;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0F;
    viewport.maxDepth = 1.0F;

    VkRect2D scissor{};
    scissor.offset = VkOffset2D{0, 0};
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewport_state_create_info{};
    {
        auto & v = viewport_state_create_info;
        v.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        v.viewportCount = 1;
        v.pViewports = &viewport;
        v.scissorCount = 1;
        v.pScissors = &scissor;
    }

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{};
    {
        auto & r = rasterization_state_create_info;
        r.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        r.depthClampEnable = VK_FALSE;
        r.rasterizerDiscardEnable = VK_FALSE;
        r.polygonMode = VK_POLYGON_MODE_FILL;
        r.lineWidth = 1.0F;
        r.cullMode = VK_CULL_MODE_BACK_BIT;
        r.frontFace = VK_FRONT_FACE_CLOCKWISE;
        r.depthBiasEnable = VK_FALSE;
        r.depthBiasConstantFactor = 0.0F;
        r.depthBiasClamp = 0.0F;
        r.depthBiasSlopeFactor = 0.0F;
    }

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info{};
    {
        auto & m = multisample_state_create_info;
        m.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m.sampleShadingEnable = VK_FALSE;
        m.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        m.minSampleShading = 1.0F;
        m.pSampleMask = nullptr;
        m.alphaToCoverageEnable = VK_FALSE;
        m.alphaToOneEnable = VK_FALSE;
    }

    VkPipelineColorBlendAttachmentState color_blend_attachment{};
    {
        auto & cba = color_blend_attachment;
        cba.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        cba.blendEnable = VK_FALSE;
        cba.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        cba.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        cba.colorBlendOp = VK_BLEND_OP_ADD;
        cba.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        cba.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        cba.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info{};
    {
        auto & cb = color_blend_state_create_info;
        cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        cb.logicOpEnable = VK_FALSE;
        cb.logicOp = VK_LOGIC_OP_COPY;
        cb.attachmentCount = 1;
        cb.pAttachments = &color_blend_attachment;
        cb.blendConstants[0] = 0.0F;
        cb.blendConstants[1] = 0.0F;
        cb.blendConstants[2] = 0.0F;
        cb.blendConstants[3] = 0.0F;
    }

    auto const dynamic_states =
        to_array({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH});

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
    {
        auto & d = dynamic_state_create_info;
        d.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        d.dynamicStateCount = dynamic_states.size();
        d.pDynamicStates = dynamic_states.data();
    }

    VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
    {
        auto & pli = pipeline_layout_create_info;
        pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pli.setLayoutCount = 0;
        pli.pSetLayouts = nullptr;
        pli.pushConstantRangeCount = 0;
        pli.pPushConstantRanges = nullptr;
    }
    auto layout =
        PipelineLayoutWrapper::create(dev, pipeline_layout_create_info);
    if (!layout.has_value()) {
        return std::nullopt;
    }

    VkAttachmentDescription color_attachment{};
    {
        auto & ca = color_attachment;
        ca.format = format;
        ca.samples = VK_SAMPLE_COUNT_1_BIT;
        ca.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        ca.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        ca.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        ca.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        ca.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        ca.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }

    VkAttachmentReference const color_ref{
        0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

    VkSubpassDescription subpass{};
    {
        auto & s = subpass;
        s.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        s.colorAttachmentCount = 1;
        s.pColorAttachments = &color_ref;
    }

    auto render_pass =
        RenderPassWrapper::create(dev, {color_attachment}, {subpass});
    if (!render_pass.has_value()) {
        return std::nullopt;
    }

    VkGraphicsPipelineCreateInfo pipeline_create_info{};
    {
        auto & p = pipeline_create_info;
        p.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        p.stageCount = shader_stage_create_infos.size();
        p.pStages = shader_stage_create_infos.data();
        p.pVertexInputState = &vertex_input_state_create_info;
        p.pInputAssemblyState = &input_assembly_state_create_info;
        p.pViewportState = &viewport_state_create_info;
        p.pRasterizationState = &rasterization_state_create_info;
        p.pMultisampleState = &multisample_state_create_info;
        p.pDepthStencilState = nullptr;
        p.pColorBlendState = &color_blend_state_create_info;
        p.pDynamicState = nullptr;
        p.layout = (*layout).get();
        p.renderPass = (*render_pass).get();
        p.subpass = 0;
        p.basePipelineHandle = VK_NULL_HANDLE;
        p.basePipelineIndex = -1;
    }

    PipelineHandle handle{};
    handle.device = dev;
    if (vkCreateGraphicsPipelines(
            dev, nullptr, 1, &pipeline_create_info, nullptr, &handle.pipeline) !=
        VK_SUCCESS) {
        return std::nullopt;
    }

    auto result = std::make_optional<PipelineWrapper>();
    (*result).setHandle(handle);
    (*result).shaders_ = std::move(shaders);
    (*result).render_pass_ = std::move(*render_pass);
    (*result).layout_ = std::move(*layout);

    fmt::print("Created VkPipeline {}\n", static_cast<void *>(handle.pipeline));

    return result;
}

void
PipelineWrapper::closeHandle(PipelineHandle handle)
{
    if (handle.device != nullptr && handle.pipeline != nullptr) {
        fmt::print("Destroying VkPipeline {}\n",
                   static_cast<void *>(handle.pipeline));
        vkDestroyPipeline(handle.device, handle.pipeline, nullptr);
    }
}

auto
PipelineWrapper::get() const -> VkPipeline
{
    return getHandle().pipeline;
}

} // namespace vulkan