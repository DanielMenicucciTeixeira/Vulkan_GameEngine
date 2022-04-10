#include "VulkanPipelineManager.h"
#include "VulkanManager.h"
#include "Renderers/Vertex.h"
#include "Renderers/Materials/Material.h"
#include "LevelGraph.h"
#include "Renderers/Materials/VulkanMaterial.h"
#include "VulkanSupport.h"

#include <vulkan/vulkan.h>
#include <fstream>
#include <iostream>

#define DEFAULT_SHADER_PATH "Engine/Shaders/"
#define DEFAULT_SHADER_EXT ".spv"

VulkanPipelineManager::VulkanPipelineManager(VulkanManager* manager)
{
	Manager = manager;
}

void VulkanPipelineManager::CreateGraphicsPipelines()
{
    for (const auto& shader : Manager->GetRenderData()->DataMapByShader)
    {
        Material* material = (*shader.second.begin()).first;
        if (M_VulkanMaterial* vulkanMaterial = dynamic_cast<M_VulkanMaterial*>(material)) CreatePipelineFromVulkanMaterial(shader.first, vulkanMaterial);
        else CreatePipelineFromGenericMaterial(shader.first, material);
    } 
}

VkVertexInputBindingDescription VulkanPipelineManager::GetBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(S_Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> VulkanPipelineManager::GetAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

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

std::vector<char> VulkanPipelineManager::ReadFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule_T* VulkanPipelineManager::CreateShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(Manager->GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

VkShaderStageFlagBits VulkanPipelineManager::GetVulkanShaderStageFlag(E_ShaderStage stage) const
{
    switch (stage)
    {
    case E_ShaderStage::VERTEX_BIT:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case E_ShaderStage::TESSELLATION_CONTROL_BIT:
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case E_ShaderStage::TESSELLATION_EVALUATION_BIT:
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case E_ShaderStage::GEOMETRY_BIT:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case E_ShaderStage::FRAGMENT_BIT:
        return  VK_SHADER_STAGE_FRAGMENT_BIT;
    case E_ShaderStage::COMPUTE_BIT:
        return VK_SHADER_STAGE_COMPUTE_BIT;
    case E_ShaderStage::ALL_GRAPHICS:
        return  VK_SHADER_STAGE_ALL_GRAPHICS;
    default:
        return VK_SHADER_STAGE_VERTEX_BIT;
    } 
}

void VulkanPipelineManager::CreatePipelineFromGenericMaterial(std::string shader, Material* material)
{
    //Create Shader Modules and Info
    std::vector<VkShaderModule> shaderModules;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    for (const auto& file : material->GetShaderInfo().ShaderFiles)
    {
        std::string filePath = DEFAULT_SHADER_PATH + file.second + DEFAULT_SHADER_EXT;
        shaderModules.push_back(CreateShaderModule(ReadFile(filePath)));

        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = GetVulkanShaderStageFlag(file.first);
        shaderStageInfo.module = *(shaderModules.end() - 1);
        shaderStageInfo.pName = "main";

        shaderStages.push_back(shaderStageInfo);
    }
    //----------------------------------------------------------------------------

    //Create PipelineLayout and Info
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    VkDescriptorSetLayout temp = Manager->GetDescriptorSetLayoutsByShader()[shader];
    pipelineLayoutInfo.pSetLayouts = &temp;
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(Manager->GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &PipelinesMap[material->GetShaderInfo().Name].second) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
    //-------------------------------------------------------------------------------

    //Create Descriptor and Info
    auto bindingDescription = GetBindingDescription();
    auto attributeDescriptions = GetAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;


    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = NULL;
    viewportState.scissorCount = 1;
    viewportState.pScissors = NULL;

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

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

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

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT,  VK_DYNAMIC_STATE_LINE_WIDTH, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 3;
    dynamicState.pDynamicStates = dynamicStates;

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

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = PipelinesMap[material->GetShaderInfo().Name].second;
    pipelineInfo.renderPass = Manager->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(Manager->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &PipelinesMap[material->GetShaderInfo().Name].first) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    for (auto& module : shaderModules) vkDestroyShaderModule(Manager->GetLogicalDevice(), module, nullptr);
}

void VulkanPipelineManager::CreatePipelineFromVulkanMaterial(std::string shader, M_VulkanMaterial* material)
{
    //Create Shader Modules and Info
    std::vector<VkShaderModule> shaderModules;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    for (const auto& file : material->GetShaderInfo().ShaderFiles)
    {
        std::string filePath = DEFAULT_SHADER_PATH + file.second + DEFAULT_SHADER_EXT;
        shaderModules.push_back(CreateShaderModule(ReadFile(filePath)));

        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = GetVulkanShaderStageFlag(file.first);
        shaderStageInfo.module = *(shaderModules.end() - 1);
        shaderStageInfo.pName = "main";

        shaderStages.push_back(shaderStageInfo);
    }
    //----------------------------------------------------------------------------

    //Set DescriptorSetLayout
    VkDescriptorSetLayout temp = Manager->GetDescriptorSetLayoutsByShader()[shader];
    material->SetDescriptorSetLayoutInPipelineLayout(temp);

    if (vkCreatePipelineLayout(Manager->GetLogicalDevice(), material->GetPipelineLayoutCreateInfo(), nullptr, &PipelinesMap[material->GetShaderInfo().Name].second) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
    //-------------------------------------------------------------------------------

    //Set Layout and Render Pass
    material->FillGraphicsPipeline(Manager->GetRenderPass(), PipelinesMap[material->GetShaderInfo().Name].second, shaderStages.size(), shaderStages.data());
    auto stages = material->GetGraphicsPipelineStages();

    //Create Graphics Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = stages.VertexInput;
    pipelineInfo.pInputAssemblyState = stages.InputAssembly;
    pipelineInfo.pViewportState = stages.Viewport;
    pipelineInfo.pRasterizationState = stages.Rasterization;
    pipelineInfo.pMultisampleState = stages.Multisample;
    pipelineInfo.pDepthStencilState = stages.DepthStencil;
    pipelineInfo.pColorBlendState = stages.ColorBlend;
    pipelineInfo.pDynamicState = stages.DynamicStatesInfo;
    pipelineInfo.layout = PipelinesMap[material->GetShaderInfo().Name].second;
    pipelineInfo.renderPass = Manager->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(Manager->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &PipelinesMap[material->GetShaderInfo().Name].first) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    for (auto& module : shaderModules) vkDestroyShaderModule(Manager->GetLogicalDevice(), module, nullptr);
}
