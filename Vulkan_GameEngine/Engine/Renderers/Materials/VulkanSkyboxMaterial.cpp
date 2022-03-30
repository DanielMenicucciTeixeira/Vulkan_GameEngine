#include "VulkanSkyboxMaterial.h"
#include "Renderers/RenderObject.h"
#include "Renderers/Vulkan/VulkanSupport.h"
#include <vulkan/vulkan.h>

MV_SkyboxMaterial::MV_SkyboxMaterial() : M_VulkanMaterial()
{
    Shader.Name = "Skybox";
    Shader.ShaderFiles.push_back(std::make_pair(E_ShaderStage::VERTEX_BIT, "SkyboxShaderVert"));
    Shader.ShaderFiles.push_back(std::make_pair(E_ShaderStage::FRAGMENT_BIT, "SkyboxShaderFrag"));
    MaterialName = "M_Skybox";

    CubeMap.Info.Stage = E_ShaderStage::FRAGMENT_BIT;
    CubeMap.Info.Type = E_ShaderVariableType::COMBINED_IMAGE_SAMPLER;
    CubeMap.Info.Subtype = E_SharderVariableSubtype::COMBINED_IMAGE_SAMPLER_CUBE;

    GraphicsPipelineStages->DepthStencil->depthTestEnable = VK_TRUE;
    GraphicsPipelineStages->DepthStencil->depthWriteEnable = VK_TRUE;
    GraphicsPipelineStages->DepthStencil->depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;

    GraphicsPipelineStages->Rasterization->cullMode = VK_CULL_MODE_FRONT_BIT;

    VkDescriptorSetLayoutBinding cubeBinding{};
    cubeBinding.binding = 4;
    cubeBinding.descriptorCount = 1;
    cubeBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    cubeBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    cubeBinding.pImmutableSamplers = nullptr;
    DescriptorSetLayoutBindingList.push_back(cubeBinding);

    DescriptorSetLayout->bindingCount = static_cast<uint32_t>(DescriptorSetLayoutBindingList.size());
    DescriptorSetLayout->pBindings = DescriptorSetLayoutBindingList.data();
}

S_GraphicsPipelineStagesInfo MV_SkyboxMaterial::GetGraphicsPipelineStages()
{
    CleanUp();
    InitializeDefaults();

    GraphicsPipelineStages->DepthStencil->depthTestEnable = VK_TRUE;
    GraphicsPipelineStages->DepthStencil->depthWriteEnable = VK_TRUE;
    GraphicsPipelineStages->DepthStencil->depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;

    GraphicsPipelineStages->Rasterization->cullMode = VK_CULL_MODE_FRONT_BIT;

    return *GraphicsPipelineStages;
}

std::vector<void*> MV_SkyboxMaterial::GetShaderVariablesData() const
{
	return { CubeMap.Value };
}

std::vector<ShaderVariableInfo> MV_SkyboxMaterial::GetShaderVariablesInfo() const
{
	return { CubeMap.Info };
}
