#include "VulkanSupport.h"
#include "Engine/Renderers/Vertex.h"

#include <vulkan/vulkan.h>

S_GraphicsPipelineStagesInfo::S_GraphicsPipelineStagesInfo(bool generateDefaults)
{
    if (generateDefaults)
    {
        InputBindingDescription = new VkVertexInputBindingDescription(VulkanSupport::GetDefaultVertexInputBindingDescription());
        VertexInputAttributeDescriptions = VulkanSupport::GetDefaultVertexInputAttributeDescriptions();
        VertexInput = new VkPipelineVertexInputStateCreateInfo(VulkanSupport::GetDefaultPipelineVertexInputStateCreateInfo(*InputBindingDescription, VertexInputAttributeDescriptions));
        InputAssembly = new VkPipelineInputAssemblyStateCreateInfo(VulkanSupport::GetDefaultPipelineInputAssemblyStateCreateInfo());
        Viewport = new VkPipelineViewportStateCreateInfo(VulkanSupport::GetDefaultPipelineViewportStateCreateInfo());
        Rasterization = new VkPipelineRasterizationStateCreateInfo(VulkanSupport::GetDefaultPipelineRasterizationStateCreateInfo());
        Multisample = new VkPipelineMultisampleStateCreateInfo(VulkanSupport::GetDefaultPipelineMultisampleStateCreateInfo());
        ColorBlendAttachment = new VkPipelineColorBlendAttachmentState(VulkanSupport::GetDefaultPipelineColorBlendAttachmentState());
        ColorBlend = new VkPipelineColorBlendStateCreateInfo(VulkanSupport::GetDefaultPipelineColorBlendStateCreateInfo(*ColorBlendAttachment));
        DepthStencil = new VkPipelineDepthStencilStateCreateInfo(VulkanSupport::GetDefaultPipelineDepthStencilStateCreateInfo());
        DynamicStatesList = VulkanSupport::GetDefaultDynamicStateList();
        DynamicStatesInfo = new VkPipelineDynamicStateCreateInfo(VulkanSupport::GetDefaultPipelineDynamicStateCreateInfo(DynamicStatesList));
    }
    else
    {
        InputBindingDescription = {};
        VertexInputAttributeDescriptions = {};
        VertexInput = {};
        InputAssembly = {};
        Viewport = {};
        Rasterization = {};
        Multisample = {};
        ColorBlendAttachment = {};
        ColorBlend = {};
        DepthStencil = {};
        DynamicStatesList = {};
        DynamicStatesInfo = {};
    }
}

S_GraphicsPipelineStagesInfo::~S_GraphicsPipelineStagesInfo()
{
    /*if (InputBindingDescription) delete(InputBindingDescription);
    if (VertexInput) delete(VertexInput);
    if (InputAssembly) delete(InputAssembly);
    if (Viewport) delete(Viewport);
    if (Rasterization) delete(Rasterization);
    if (Multisample) delete(Multisample);
    if (ColorBlendAttachment) delete(ColorBlendAttachment);
    if (ColorBlend) delete(ColorBlend);
    if (DepthStencil) delete(DepthStencil);
    if (DynamicStatesInfo) delete(DynamicStatesInfo);*/
}

std::vector<VkDescriptorSetLayoutBinding> VulkanSupport::GetDefaultDescriptorSetLayoutBindingList()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings(4);

    VkDescriptorSetLayoutBinding cameraLayoutBinding{};
    cameraLayoutBinding.binding = 0;
    cameraLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    cameraLayoutBinding.descriptorCount = 1;
    cameraLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    cameraLayoutBinding.pImmutableSamplers = nullptr; // Optional
    bindings[0] = cameraLayoutBinding;

    VkDescriptorSetLayoutBinding modelLayoutBinding{};
    modelLayoutBinding.binding = 1;
    modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    modelLayoutBinding.descriptorCount = 1;
    modelLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    modelLayoutBinding.pImmutableSamplers = nullptr; // Optional
    bindings[1] = modelLayoutBinding;

    VkDescriptorSetLayoutBinding numberOfLightsLayoutBinding{};
    numberOfLightsLayoutBinding.binding = 2;
    numberOfLightsLayoutBinding.descriptorCount = 1;
    numberOfLightsLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    numberOfLightsLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    numberOfLightsLayoutBinding.pImmutableSamplers = nullptr;
    bindings[2] = numberOfLightsLayoutBinding;

    VkDescriptorSetLayoutBinding lightsLayoutBinding{};
    lightsLayoutBinding.binding = 3;
    lightsLayoutBinding.descriptorCount = 1;
    lightsLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lightsLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    lightsLayoutBinding.pImmutableSamplers = nullptr;
    bindings[3] = lightsLayoutBinding;

    return bindings;
}

VkDescriptorSetLayoutCreateInfo VulkanSupport::GetDefaultDescriptorSetLayoutCreateInfo(std::vector<VkDescriptorSetLayoutBinding>& layoutBindingsList)
{
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindingsList.size());
    layoutInfo.pBindings = layoutBindingsList.data();
    return layoutInfo;
}

VkPipelineLayoutCreateInfo VulkanSupport::GetDefaultPipelineLayoutCreateInfo()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = {};//Set in VulkanPipelineManager
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
    return pipelineLayoutInfo;
}

VkVertexInputBindingDescription VulkanSupport::GetDefaultVertexInputBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(S_Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> VulkanSupport::GetDefaultVertexInputAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    VkVertexInputAttributeDescription blanckDescription{};
    for(int i = 0; i < 3; i++) attributeDescriptions.push_back(blanckDescription);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(S_Vertex, Position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(S_Vertex, TextureCoordinates);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(S_Vertex, Normal);

    return attributeDescriptions;
}

VkPipelineVertexInputStateCreateInfo VulkanSupport::GetDefaultPipelineVertexInputStateCreateInfo(const VkVertexInputBindingDescription& bindingDescription,const  std::vector<VkVertexInputAttributeDescription>& attributeDescriptions)
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo VulkanSupport::GetDefaultPipelineInputAssemblyStateCreateInfo()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    return inputAssembly;
}

VkPipelineViewportStateCreateInfo VulkanSupport::GetDefaultPipelineViewportStateCreateInfo()
{
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = NULL;
    viewportState.scissorCount = 1;
    viewportState.pScissors = NULL;
    return viewportState;
}

VkPipelineRasterizationStateCreateInfo VulkanSupport::GetDefaultPipelineRasterizationStateCreateInfo()
{
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
    return rasterizer;
}

VkPipelineMultisampleStateCreateInfo VulkanSupport::GetDefaultPipelineMultisampleStateCreateInfo()
{
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
    return  multisampling;
}

VkPipelineColorBlendAttachmentState VulkanSupport::GetDefaultPipelineColorBlendAttachmentState()
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    return colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo VulkanSupport::GetDefaultPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentState& colorBlendAttachment)
{
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional
    return colorBlending;
}

VkPipelineDepthStencilStateCreateInfo VulkanSupport::GetDefaultPipelineDepthStencilStateCreateInfo()
{
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional
    return depthStencil;
}

std::vector<VkDynamicState> VulkanSupport::GetDefaultDynamicStateList()
{
    return { VK_DYNAMIC_STATE_VIEWPORT,  VK_DYNAMIC_STATE_LINE_WIDTH, VK_DYNAMIC_STATE_SCISSOR };
}

VkPipelineDynamicStateCreateInfo VulkanSupport::GetDefaultPipelineDynamicStateCreateInfo(std::vector<VkDynamicState>& dynamicStatesList)
{
    VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.dynamicStateCount = dynamicStatesList.size();
    dynamicStateInfo.pDynamicStates = dynamicStatesList.data();
    return dynamicStateInfo;
}

VkGraphicsPipelineCreateInfo VulkanSupport::GetDefaultGraphicsPipelineCreateInfo(const S_GraphicsPipelineStagesInfo& stagesInfo)
{
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = {};//Set in VulkanPipelineManager
    pipelineInfo.pStages = {};//Set in VulkanPipelineManager
    pipelineInfo.pVertexInputState = stagesInfo.VertexInput;
    pipelineInfo.pInputAssemblyState = stagesInfo.InputAssembly;
    pipelineInfo.pViewportState = stagesInfo.Viewport;
    pipelineInfo.pRasterizationState = stagesInfo.Rasterization;
    pipelineInfo.pMultisampleState = stagesInfo.Multisample;
    pipelineInfo.pDepthStencilState = stagesInfo.DepthStencil;
    pipelineInfo.pColorBlendState = stagesInfo.ColorBlend;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = {};//Set in VulkanPipelineManager
    pipelineInfo.renderPass = {};//Set in VulkanPipelineManager
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional
    pipelineInfo.pDynamicState = stagesInfo.DynamicStatesInfo;
    return pipelineInfo;
}
