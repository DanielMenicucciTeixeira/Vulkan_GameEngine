#ifndef VULKANSUPPORT_H
#define VULKANSUPPORT_H

#include <vector>

struct VkDescriptorSetLayoutBinding;
struct VkDescriptorSetLayoutCreateInfo;
struct VkPipelineLayoutCreateInfo;
struct VkVertexInputBindingDescription;
struct VkDescriptorSetLayout_T;
struct VkVertexInputAttributeDescription;
struct VkPipelineVertexInputStateCreateInfo;
struct VkPipelineInputAssemblyStateCreateInfo;
struct VkPipelineViewportStateCreateInfo;
struct VkPipelineRasterizationStateCreateInfo;
struct VkPipelineMultisampleStateCreateInfo;
struct VkPipelineColorBlendAttachmentState;
struct VkPipelineColorBlendStateCreateInfo;
struct VkPipelineDepthStencilStateCreateInfo;
struct VkPipelineLayoutCreateInfo;
struct VkPipelineDynamicStateCreateInfo;
struct VkGraphicsPipelineCreateInfo;
struct VkWriteDescriptorSet;
enum VkDynamicState;

struct S_GraphicsPipelineStagesInfo
{
	VkVertexInputBindingDescription* InputBindingDescription;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
	VkPipelineVertexInputStateCreateInfo* VertexInput;
	VkPipelineInputAssemblyStateCreateInfo* InputAssembly;
	VkPipelineViewportStateCreateInfo* Viewport;
	VkPipelineRasterizationStateCreateInfo* Rasterization;
	VkPipelineMultisampleStateCreateInfo* Multisample;
	VkPipelineColorBlendAttachmentState* ColorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo* ColorBlend;
	VkPipelineDepthStencilStateCreateInfo* DepthStencil;
	std::vector<VkDynamicState> DynamicStatesList;
	VkPipelineDynamicStateCreateInfo* DynamicStatesInfo;

	S_GraphicsPipelineStagesInfo(bool generateDefaults = true);
	virtual ~S_GraphicsPipelineStagesInfo();
};

class VulkanSupport
{
public:
	VulkanSupport() = delete;
	~VulkanSupport() = delete;
	VulkanSupport operator= (VulkanSupport) = delete;
	VulkanSupport* operator= (VulkanSupport*) = delete;

	//Descriptor Sets Functions

	static std::vector<VkDescriptorSetLayoutBinding> GetDefaultDescriptorSetLayoutBindingList();
	static VkDescriptorSetLayoutCreateInfo GetDefaultDescriptorSetLayoutCreateInfo(std::vector<VkDescriptorSetLayoutBinding>& layoutBindingsList);

	//Graphics Pipeline Functions

	static VkPipelineLayoutCreateInfo GetDefaultPipelineLayoutCreateInfo();
	static VkVertexInputBindingDescription GetDefaultVertexInputBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> GetDefaultVertexInputAttributeDescriptions();
	static VkPipelineVertexInputStateCreateInfo GetDefaultPipelineVertexInputStateCreateInfo(const VkVertexInputBindingDescription& bindingDescription,const std::vector<VkVertexInputAttributeDescription>& attributeDescriptions);
	static VkPipelineInputAssemblyStateCreateInfo GetDefaultPipelineInputAssemblyStateCreateInfo();
	static VkPipelineViewportStateCreateInfo GetDefaultPipelineViewportStateCreateInfo();
	static VkPipelineRasterizationStateCreateInfo GetDefaultPipelineRasterizationStateCreateInfo();
	static VkPipelineMultisampleStateCreateInfo GetDefaultPipelineMultisampleStateCreateInfo();
	static VkPipelineColorBlendAttachmentState GetDefaultPipelineColorBlendAttachmentState();
	static VkPipelineColorBlendStateCreateInfo GetDefaultPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentState& colorBlendAttachment);
	static VkPipelineDepthStencilStateCreateInfo GetDefaultPipelineDepthStencilStateCreateInfo();
	static std::vector<VkDynamicState> GetDefaultDynamicStateList();
	static VkPipelineDynamicStateCreateInfo GetDefaultPipelineDynamicStateCreateInfo(std::vector<VkDynamicState>& dynamicStatesList);
	
	static VkGraphicsPipelineCreateInfo GetDefaultGraphicsPipelineCreateInfo(const S_GraphicsPipelineStagesInfo& stagesInfo);
};

#endif
