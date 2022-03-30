#ifndef VULKANPIPELINEMANAGER_H
#define VULKANPIPELINEMANAGER_H

#include <vector>
#include <set>
#include <string>
#include <array>
#include <unordered_map>

class VulkanManager;
class Material;
class M_VulkanMaterial;

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

protected:

	static std::vector<char> ReadFile(const std::string& filename);
	VkShaderModule_T* CreateShaderModule(const std::vector<char>& code);

	std::unordered_map <std::string, std::pair<VkPipeline_T*, VkPipelineLayout_T*>> PipelinesMap;
	VulkanManager* Manager = nullptr;

	VkShaderStageFlagBits GetVulkanShaderStageFlag(E_ShaderStage stage) const;

	void CreatePipelineFromGenericMaterial(std::string shader, Material* material);
	void CreatePipelineFromVulkanMaterial(std::string shader, M_VulkanMaterial* material);
};
#endif

