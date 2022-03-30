#include "VulkanMaterial.h"
#include "Renderers/Vulkan/VulkanSupport.h"
#include <vulkan/vulkan.h>

M_VulkanMaterial::M_VulkanMaterial(bool initilizeDefaultValues) : Material()
{
	InitializeDefaultValues = initilizeDefaultValues;
	if (InitializeDefaultValues) InitializeDefaults();
	else InitizlizeBlanck();
}

M_VulkanMaterial::~M_VulkanMaterial()
{
	CleanUp();
}

std::vector<void*> M_VulkanMaterial::GetShaderVariablesData() const
{
	return std::vector<void*>();
}

std::vector<ShaderVariableInfo> M_VulkanMaterial::GetShaderVariablesInfo() const
{
	return std::vector<ShaderVariableInfo>();
}

S_GraphicsPipelineStagesInfo M_VulkanMaterial::GetGraphicsPipelineStages()
{
	CleanUp();
	if (InitializeDefaultValues) InitializeDefaults();
	else InitizlizeBlanck();
	
	return *GraphicsPipelineStages;
}

void M_VulkanMaterial::SetDescriptorSetLayoutInPipelineLayout(VkDescriptorSetLayout_T*& descriptorSet)
{
	PipelineLayout->pSetLayouts = &descriptorSet;
}

void M_VulkanMaterial::FillGraphicsPipeline(VkRenderPass_T* renderPass, VkPipelineLayout_T* pipelineLayout, uint32_t stageCount, VkPipelineShaderStageCreateInfo* stages)
{
	GraphicsPipeline->stageCount = stageCount;
	GraphicsPipeline->pStages = stages;
	GraphicsPipeline->renderPass = renderPass;
	GraphicsPipeline->layout = pipelineLayout;
}

void M_VulkanMaterial::CleanUp()
{
	if (GraphicsPipelineStages) delete(GraphicsPipelineStages);
	if (DescriptorSetLayout) delete(DescriptorSetLayout);
	if (GraphicsPipeline) delete(GraphicsPipeline);
	if (PipelineLayout) delete(PipelineLayout);
	DescriptorSetLayoutBindingList.clear();
}

void M_VulkanMaterial::InitializeDefaults()
{
	GraphicsPipelineStages = new S_GraphicsPipelineStagesInfo();
	DescriptorSetLayoutBindingList = VulkanSupport::GetDefaultDescriptorSetLayoutBindingList();
	DescriptorSetLayout = new VkDescriptorSetLayoutCreateInfo(VulkanSupport::GetDefaultDescriptorSetLayoutCreateInfo(DescriptorSetLayoutBindingList));
	GraphicsPipeline = new VkGraphicsPipelineCreateInfo(VulkanSupport::GetDefaultGraphicsPipelineCreateInfo(GraphicsPipelineStages));
	PipelineLayout = new VkPipelineLayoutCreateInfo(VulkanSupport::GetDefaultPipelineLayoutCreateInfo());
}

void M_VulkanMaterial::InitizlizeBlanck()
{
	GraphicsPipelineStages = new S_GraphicsPipelineStagesInfo(false);
	DescriptorSetLayoutBindingList = {};
	DescriptorSetLayout = nullptr;
	GraphicsPipeline = nullptr;
	PipelineLayout = nullptr;
}
