#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include <vector>
#include <string>
#include <array>

class VulkanManager;

struct VkShaderModule_T;
struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;
struct VkPipelineLayout_T;
struct VkPipeline_T;

class VulkanPipeline
{
public:
	VulkanPipeline(VulkanManager* manager);

	void CreateGraphicsPipeline();
	static VkVertexInputBindingDescription GetBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

	inline VkPipeline_T* GetPipeline() { return Pipeline; }
	inline VkPipelineLayout_T* GetPipelineLayout() { return PipelineLayout; }

protected:

	static std::vector<char> ReadFile(const std::string& filename);
	VkShaderModule_T* CreateShaderModule(const std::vector<char>& code);
	VkPipelineLayout_T* PipelineLayout = nullptr;
	VkPipeline_T* Pipeline = nullptr;
	
	VulkanManager* Manager = nullptr;
};
#endif

