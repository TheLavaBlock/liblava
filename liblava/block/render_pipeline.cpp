/**
 * @file         liblava/block/render_pipeline.cpp
 * @brief        Render pipeline (Graphics)
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#include "liblava/block/render_pipeline.hpp"
#include "liblava/util/log.hpp"

namespace lava {

//-----------------------------------------------------------------------------
render_pipeline::render_pipeline(device::ptr dev,
                                 VkPipelineCache pipeline_cache)
: pipeline(dev, pipeline_cache) {
    m_info.vertex_input_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    m_info.vertex_input_state.pNext = nullptr;
    m_info.vertex_input_state.flags = 0;
    m_info.vertex_input_state.vertexBindingDescriptionCount = 0;
    m_info.vertex_input_state.pVertexBindingDescriptions = nullptr;
    m_info.vertex_input_state.vertexAttributeDescriptionCount = 0;
    m_info.vertex_input_state.pVertexAttributeDescriptions = nullptr;

    m_info.input_assembly_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_info.input_assembly_state.pNext = nullptr;
    m_info.input_assembly_state.flags = 0;
    m_info.input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    m_info.input_assembly_state.primitiveRestartEnable = VK_FALSE;

    m_info.viewport_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    m_info.viewport_state.pNext = nullptr;
    m_info.viewport_state.flags = 0;
    m_info.viewport_state.viewportCount = 1;
    m_info.viewport_state.pViewports = nullptr;
    m_info.viewport_state.scissorCount = 1;
    m_info.viewport_state.pScissors = nullptr;

    m_info.multisample_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_info.multisample_state.pNext = nullptr;
    m_info.multisample_state.flags = 0;
    m_info.multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    m_info.multisample_state.sampleShadingEnable = VK_FALSE;
    m_info.multisample_state.minSampleShading = 0.f;
    m_info.multisample_state.pSampleMask = nullptr;
    m_info.multisample_state.alphaToCoverageEnable = VK_FALSE;
    m_info.multisample_state.alphaToOneEnable = VK_FALSE;

    m_info.depth_stencil_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    m_info.depth_stencil_state.pNext = nullptr;
    m_info.depth_stencil_state.flags = 0;
    m_info.depth_stencil_state.depthTestEnable = VK_FALSE;
    m_info.depth_stencil_state.depthWriteEnable = VK_FALSE;
    m_info.depth_stencil_state.depthCompareOp = VK_COMPARE_OP_NEVER;
    m_info.depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
    m_info.depth_stencil_state.stencilTestEnable = VK_FALSE;
    m_info.depth_stencil_state.front = {};
    m_info.depth_stencil_state.back = {};
    m_info.depth_stencil_state.minDepthBounds = 0.f;
    m_info.depth_stencil_state.maxDepthBounds = 0.f;

    m_info.rasterization_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_info.rasterization_state.pNext = nullptr;
    m_info.rasterization_state.flags = 0;
    m_info.rasterization_state.depthClampEnable = VK_FALSE;
    m_info.rasterization_state.rasterizerDiscardEnable = VK_FALSE;
    m_info.rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
    m_info.rasterization_state.cullMode = VK_CULL_MODE_NONE;
    m_info.rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    m_info.rasterization_state.depthBiasEnable = VK_FALSE;
    m_info.rasterization_state.depthBiasConstantFactor = 0.f;
    m_info.rasterization_state.depthBiasClamp = 0.f;
    m_info.rasterization_state.depthBiasSlopeFactor = 0.f;
    m_info.rasterization_state.lineWidth = 1.f;

    m_color_blend_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_color_blend_state.pNext = nullptr;
    m_color_blend_state.flags = 0;
    m_color_blend_state.logicOpEnable = VK_FALSE;
    m_color_blend_state.logicOp = VK_LOGIC_OP_CLEAR;
    m_color_blend_state.attachmentCount = 0;
    m_color_blend_state.pAttachments = nullptr;
    m_color_blend_state.blendConstants[0] = 0.f;
    m_color_blend_state.blendConstants[1] = 0.f;
    m_color_blend_state.blendConstants[2] = 0.f;
    m_color_blend_state.blendConstants[3] = 0.f;

    m_dynamic_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_dynamic_state.pNext = nullptr;
    m_dynamic_state.flags = 0;
    m_dynamic_state.dynamicStateCount = 0;
    m_dynamic_state.pDynamicStates = nullptr;

    set_dynamic_states({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR});
}

//-----------------------------------------------------------------------------
void render_pipeline::set_vertex_input_binding(
    VkVertexInputBindingDescription const& description) {
    VkVertexInputBindingDescriptions descriptions;
    descriptions.push_back(description);

    set_vertex_input_bindings(descriptions);
}

//-----------------------------------------------------------------------------
void render_pipeline::set_vertex_input_bindings(
    VkVertexInputBindingDescriptions const& descriptions) {
    m_vertex_input_bindings = descriptions;

    m_info.vertex_input_state.vertexBindingDescriptionCount =
        to_ui32(m_vertex_input_bindings.size());
    m_info.vertex_input_state.pVertexBindingDescriptions =
        m_vertex_input_bindings.data();
}

//-----------------------------------------------------------------------------
void render_pipeline::set_vertex_input_attribute(
    VkVertexInputAttributeDescription const& attribute) {
    VkVertexInputAttributeDescriptions attributes;
    attributes.push_back(attribute);

    set_vertex_input_attributes(attributes);
}

//-----------------------------------------------------------------------------
void render_pipeline::set_vertex_input_attributes(
    VkVertexInputAttributeDescriptions const& attributes) {
    m_vertex_input_attributes = attributes;

    m_info.vertex_input_state.vertexAttributeDescriptionCount =
        to_ui32(m_vertex_input_attributes.size());
    m_info.vertex_input_state.pVertexAttributeDescriptions =
        m_vertex_input_attributes.data();
}

//-----------------------------------------------------------------------------
void render_pipeline::set_input_topology(VkPrimitiveTopology const& topology) {
    m_info.input_assembly_state.topology = topology;
}

//-----------------------------------------------------------------------------
void render_pipeline::set_depth_test_and_write(bool enable_test,
                                               bool enable_write) {
    m_info.depth_stencil_state.depthTestEnable = enable_test
                                                     ? VK_TRUE
                                                     : VK_FALSE;
    m_info.depth_stencil_state.depthWriteEnable = enable_write
                                                      ? VK_TRUE
                                                      : VK_FALSE;
}

//-----------------------------------------------------------------------------
void render_pipeline::set_depth_compare_op(VkCompareOp compare_op) {
    m_info.depth_stencil_state.depthCompareOp = compare_op;
}

//-----------------------------------------------------------------------------
void render_pipeline::set_rasterization_cull_mode(VkCullModeFlags cull_mode) {
    m_info.rasterization_state.cullMode = cull_mode;
}

//-----------------------------------------------------------------------------
void render_pipeline::set_rasterization_front_face(VkFrontFace front_face) {
    m_info.rasterization_state.frontFace = front_face;
}

//-----------------------------------------------------------------------------
void render_pipeline::set_rasterization_polygon_mode(VkPolygonMode polygon_mode) {
    m_info.rasterization_state.polygonMode = polygon_mode;
}

//-----------------------------------------------------------------------------
void render_pipeline::add_color_blend_attachment(
    VkPipelineColorBlendAttachmentState const& attachment) {
    m_color_blend_attachment_states.push_back(attachment);

    m_color_blend_state.attachmentCount = to_ui32(m_color_blend_attachment_states.size());
    m_color_blend_state.pAttachments = m_color_blend_attachment_states.data();
}

//-----------------------------------------------------------------------------
void render_pipeline::add_color_blend_attachment() {
    add_color_blend_attachment(create_pipeline_color_blend_attachment());
}

//-----------------------------------------------------------------------------
void render_pipeline::clear_color_blend_attachment() {
    m_color_blend_attachment_states.clear();

    m_color_blend_state.attachmentCount = 0;
    m_color_blend_state.pAttachments = nullptr;
}

//-----------------------------------------------------------------------------
void render_pipeline::set_dynamic_states(VkDynamicStates const& states) {
    m_dynamic_states = states;

    m_dynamic_state.dynamicStateCount = to_ui32(m_dynamic_states.size());
    m_dynamic_state.pDynamicStates = m_dynamic_states.data();
}

//-----------------------------------------------------------------------------
void render_pipeline::add_dynamic_state(VkDynamicState state) {
    m_dynamic_states.push_back(state);
    set_dynamic_states(m_dynamic_states);
}

//-----------------------------------------------------------------------------
void render_pipeline::clear_dynamic_states() {
    m_dynamic_states.clear();

    m_dynamic_state.dynamicStateCount = 0;
    m_dynamic_state.pDynamicStates = nullptr;
}

//-----------------------------------------------------------------------------
bool render_pipeline::add_shader_stage(c_data::ref data,
                                       VkShaderStageFlagBits stage) {
    if (!data.addr) {
        logger()->error("graphics pipeline shader stage data");
        return false;
    }

    auto shader_stage = create_pipeline_shader_stage(m_device,
                                                     data,
                                                     stage);
    if (!shader_stage) {
        logger()->error("create graphics pipeline shader stage");
        return false;
    }

    add(shader_stage);
    return true;
}

//-----------------------------------------------------------------------------
void render_pipeline::copy_to(render_pipeline* target) const {
    target->set_layout(m_layout);

    target->m_info = m_info;

    target->m_shader_stages = m_shader_stages;
    target->m_vertex_input_bindings = m_vertex_input_bindings;
    target->m_vertex_input_attributes = m_vertex_input_attributes;

    target->m_color_blend_attachment_states = m_color_blend_attachment_states;
    target->m_color_blend_state = m_color_blend_state;
    target->m_dynamic_states = m_dynamic_states;
}

//-----------------------------------------------------------------------------
bool render_pipeline::setup() {
    if (on_create && !on_create(m_info))
        return false;

    VkPipelineShaderStageCreateInfos stages;

    for (auto& shader_stage : m_shader_stages)
        stages.push_back(shader_stage->get_create_info());

    VkGraphicsPipelineCreateInfo const vk_create_info{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = to_ui32(stages.size()),
        .pStages = stages.data(),
        .pVertexInputState = &m_info.vertex_input_state,
        .pInputAssemblyState = &m_info.input_assembly_state,
        .pTessellationState = nullptr,
        .pViewportState = &m_info.viewport_state,
        .pRasterizationState = &m_info.rasterization_state,
        .pMultisampleState = &m_info.multisample_state,
        .pDepthStencilState = &m_info.depth_stencil_state,
        .pColorBlendState = &m_color_blend_state,
        .pDynamicState = &m_dynamic_state,
        .layout = m_layout->get(),
        .renderPass = m_render_pass,
        .subpass = to_ui32(m_subpass),
        .basePipelineHandle = 0,
        .basePipelineIndex = undef,
    };

    std::array<VkGraphicsPipelineCreateInfo, 1> const vk_info = {vk_create_info};

    return check(m_device->call().vkCreateGraphicsPipelines(m_device->get(),
                                                            m_pipeline_cache,
                                                            to_ui32(vk_info.size()),
                                                            vk_info.data(),
                                                            memory::instance().alloc(),
                                                            &m_vk_pipeline));
}

//-----------------------------------------------------------------------------
void render_pipeline::teardown() {
    clear();
}

//-----------------------------------------------------------------------------
void render_pipeline::bind(VkCommandBuffer cmd_buf) {
    vkCmdBindPipeline(cmd_buf,
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      m_vk_pipeline);
}

//-----------------------------------------------------------------------------
void render_pipeline::set_viewport_and_scissor(VkCommandBuffer cmd_buf,
                                               uv2 size) {
    VkViewport viewportParam;
    viewportParam.x = 0.f;
    viewportParam.y = 0.f;
    viewportParam.width = to_r32(size.x);
    viewportParam.height = to_r32(size.y);
    viewportParam.minDepth = 0.f;
    viewportParam.maxDepth = 1.f;

    VkRect2D scissorParam;
    scissorParam.offset = {0, 0};
    scissorParam.extent = {size.x, size.y};

    if (m_sizing == sizing_mode::absolute) {
        viewportParam = m_viewport;
        scissorParam = m_scissor;
    } else if (m_sizing == sizing_mode::relative) {
        viewportParam.x = m_viewport.x * size.x;
        viewportParam.y = m_viewport.y * size.y;
        viewportParam.width = m_viewport.width * size.x;
        viewportParam.height = m_viewport.height * size.y;

        scissorParam.offset.x = m_scissor.offset.x * size.x;
        scissorParam.offset.y = m_scissor.offset.y * size.y;
        scissorParam.extent.width = m_scissor.extent.width * size.x;
        scissorParam.extent.height = m_scissor.extent.height * size.y;
    } else {
        m_viewport = viewportParam;
        m_scissor = scissorParam;
    }

    std::array<VkViewport, 1> const viewports = {viewportParam};
    vkCmdSetViewport(cmd_buf,
                     0,
                     to_ui32(viewports.size()),
                     viewports.data());

    std::array<VkRect2D, 1> const scissors = {scissorParam};
    vkCmdSetScissor(cmd_buf,
                    0,
                    to_ui32(scissors.size()),
                    scissors.data());
}

//-----------------------------------------------------------------------------
VkPipelineColorBlendAttachmentState create_pipeline_color_blend_attachment() {
    return {
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                          | VK_COLOR_COMPONENT_G_BIT
                          | VK_COLOR_COMPONENT_B_BIT
                          | VK_COLOR_COMPONENT_A_BIT,
    };
}

} // namespace lava
