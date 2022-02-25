#ifndef VULKANPIPELINEMANAGER_H
#define VULKANPIPELINEMANAGER_H

#include <vector>
#include <set>
#include <string>
#include <array>
#include <unordered_map>

class VulkanManager;
class Material;

enum class E_ShaderStage;
enum VkShaderStageFlagBits;

struct VkShaderModule_T;
struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;
struct VkPipelineLayout_T;
struct VkPipeline_T;

class VulkanPipelineManager
{
public:
	VulkanPipelineManager(VulkanManager* manager);

	void CreateGraphicsPipelines();
	static VkVertexInputBindingDescription GetBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

	inline std::unordered_map<std::string, std::pair<VkPipeline_T*, VkPipelineLayout_T*>> GetPipelinesMap() { return PipelinesMap; }
	//inline VkPipelineLayout_T* GetPipelineLayout() { return PipelineLayout; }

protected:

	static std::vector<char> ReadFile(const std::string& filename);
	VkShaderModule_T* CreateShaderModule(const std::vector<char>& code);
	//VkPipelineLayout_T* PipelineLayout = nullptr;

	std::unordered_map <std::string, std::pair<VkPipeline_T*, VkPipelineLayout_T*>> PipelinesMap;
	//VkPipeline_T* Pipeline = nullptr;
	VulkanManager* Manager = nullptr;

	//std::string GetShaderBitString(E_ShaderStage stage) const;
	VkShaderStageFlagBits GetVulkanShaderStageFlag(E_ShaderStage stage) const;
};
#endif

