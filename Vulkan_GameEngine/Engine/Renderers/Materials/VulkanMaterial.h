#ifndef VULKANMATERIAL_H
#define VULKANMATERIAL_H

#include "Material.h"
#include <vector>

class VulkanSupport;
struct S_GraphicsPipelineStagesInfo;
struct VkDescriptorSetLayoutBinding;
struct VkDescriptorSetLayoutCreateInfo;
struct VkGraphicsPipelineCreateInfo;
struct VkPipelineLayoutCreateInfo;
struct VkWriteDescriptorSet;
struct VkPipelineShaderStageCreateInfo;
struct VkDescriptorSetLayout_T;
struct VkRenderPass_T;
struct VkPipelineLayout_T;

class M_VulkanMaterial : public Material
{
public:
	M_VulkanMaterial(bool initilizeDefaultValues = true);
	virtual ~M_VulkanMaterial();

	virtual std::vector<void*> GetShaderVariablesData() const override;
	virtual std::vector<ShaderVariableInfo> GetShaderVariablesInfo() const override;

	inline VkGraphicsPipelineCreateInfo* GetGraphicsPipelineCreateInfo() const { return GraphicsPipeline; }
	inline VkDescriptorSetLayoutCreateInfo* GetDescriptorSetLayoutCreateInfo() const { return DescriptorSetLayout; }
	inline VkPipelineLayoutCreateInfo* GetPipelineLayoutCreateInfo() const { return PipelineLayout; }
	virtual S_GraphicsPipelineStagesInfo GetGraphicsPipelineStages();
	void SetDescriptorSetLayoutInPipelineLayout(VkDescriptorSetLayout_T*& descriptorSet);
	void FillGraphicsPipeline(VkRenderPass_T* renderPass, VkPipelineLayout_T* pipelineLayout, uint32_t stageCount, VkPipelineShaderStageCreateInfo* stages);

protected:
	bool InitializeDefaultValues;
	std::vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindingList;
	VkDescriptorSetLayoutCreateInfo* DescriptorSetLayout;

	VkPipelineLayoutCreateInfo* PipelineLayout;
	S_GraphicsPipelineStagesInfo* GraphicsPipelineStages;
	VkGraphicsPipelineCreateInfo* GraphicsPipeline;

	void CleanUp();
	void InitializeDefaults();
	void InitizlizeBlanck();
};

#endif

