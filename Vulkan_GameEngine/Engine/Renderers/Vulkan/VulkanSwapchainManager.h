#ifndef VULKANSWAPCHAINMANAGER_H
#define VULKANSWAPCHAINMANAGER_H

#include <vector>
#include <string>
#include <unordered_map>

class VulkanManager;
class FMatrix4;
class Material;
class M_VulkanMaterial;
class M_UI_Material;
class M_UI_VulkanMaterial;

enum VkPresentModeKHR;
enum VkFormat;
enum VkImageTiling;
enum VkImageLayout;
enum VkShaderStageFlagBits;
enum VkDescriptorType;

enum class E_ShaderStage;
enum class E_ShaderVariableType;

struct VkSurfaceFormatKHR;
struct VkExtent2D;
struct VkImage_T;
struct VkImageView_T;
struct VkSurfaceCapabilitiesKHR;
struct VkSwapchainKHR_T;
struct VkFramebuffer_T;
struct VkRenderPass_T;
struct VkDeviceMemory_T;
struct VkBuffer_T;
struct VkDescriptorSetLayout_T;
struct VkDescriptorPool_T;
struct VkDescriptorSet_T;
struct VkWriteDescriptorSet;
struct VkSampler_T;
struct UniformBufferObject;
struct UniformCameraObject;
struct S_Texture;
struct S_CubeSampler;
struct ShaderVariableInfo;

typedef unsigned int VkFlags;
typedef VkFlags VkImageAspectFlags;
typedef VkFlags VkFormatFeatureFlags;
typedef VkFlags VkImageUsageFlags;
typedef VkFlags VkMemoryPropertyFlags;

typedef enum VkImageViewType;

struct S_BufferData
{
	VkBuffer_T* Buffer;
	VkDeviceMemory_T* Memory;
	void* Data;
	size_t Size;
};

struct S_TextureData
{
	VkImage_T* TextureImage = nullptr;
	VkDeviceMemory_T* TextureImageMemory = nullptr;
	VkImageView_T* TextureImageView = nullptr;
	VkSampler_T* TextureSampler = nullptr;
};

class VulkanSwapchainManager
{
public:
	VulkanSwapchainManager(VulkanManager* manager);
	~VulkanSwapchainManager();

//Functions
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities);

	void CreateSwapChain();

	void CreateImageViews();
	VkImageView_T* CreateImageView(VkImage_T* image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType imageViewType, unsigned int layerCount = 1);
	void CreateRenderPass();
	void CreateImage(unsigned int width, unsigned int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage_T*& image, VkDeviceMemory_T*& imageMemory, VkFlags flags = {}, int layerCount = 1);
	void CreateDepthResources();
	void CreateFramebuffers();
	void CreateUniformBuffers();
	void CreateDescriptorSetLayouts();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateUIDescriptorSets();
	void CreateTextureImages();
	void CreateSkyboxImages();
	void CreateTextureSampler(VkSampler_T*& sampler);

	void RecreationCleanUp();
	void FinalCleanUp();

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	void TransitionImageLayout(VkImage_T* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, int layerCount = 1);
	bool HasStencilComponent(VkFormat format);
	void CopyBufferToImage(VkBuffer_T* buffer, VkImage_T* image, unsigned int width, unsigned int height, unsigned int layerCount = 1);

	inline VkExtent2D* GetExtent() const { return Extent; }
	inline VkRenderPass_T* GetRenderPass() const { return RenderPass; }

	inline std::vector<VkFramebuffer_T*> GetFramebuffers() const { return Framebuffers; }
	inline std::unordered_map<const FMatrix4*, std::vector<VkDescriptorSet_T*>> GetDescriptorSetsMap() const { return DescriptorSetsMap; }
	inline std::unordered_map<const FMatrix4*, std::vector<VkDescriptorSet_T*>> GetUIDescriptorSetsMap() const { return UIDescriptorSetsMap; }
	inline std::vector<VkImage_T*> GetImages() const { return Images; }
	inline VkSwapchainKHR_T* GetSwapchain() const { return Swapchain; }
	inline std::unordered_map<std::string, VkDescriptorSetLayout_T*> GetDescriptorLayoutsByShader() const { return DescriptorLayoutsByShader; }
	inline std::unordered_map<std::string, VkDescriptorSetLayout_T*> GetUIDescriptorSetLayoutMap() const { return UIDescriptorSetLayoutByShader; }

	void UpdateBuffers(unsigned int currentImageIndex);


protected:
	VkShaderStageFlagBits GetVulkanShaderStageFlag(E_ShaderStage stage) const;
	VkDescriptorType GetVulkanDescriptorType(E_ShaderVariableType type) const;
	
	std::vector<VkWriteDescriptorSet> CreateDescriptorWritesFromMaterial(Material* material, int currentImage);
	VkWriteDescriptorSet CreateCombinedImageSamplerWrite(ShaderVariableInfo info, void* data);
	VkWriteDescriptorSet CreateUniformBufferWrite(Material* material, int currentImage, ShaderVariableInfo info, void* data);

	//Variables

	VkSwapchainKHR_T* Swapchain = nullptr;

	std::vector<VkImage_T*> Images;
	std::vector<VkFramebuffer_T*> Framebuffers;
	VkFormat ImageFormat;
	VkExtent2D* Extent = nullptr;
	std::vector<VkImageView_T*> ImageViews;

	VkRenderPass_T* RenderPass = nullptr;

	VkImage_T* DepthImage = nullptr;
	VkDeviceMemory_T* DepthImageMemory = nullptr;
	VkImageView_T* DepthImageView = nullptr;

	std::unordered_map<const FMatrix4*, std::vector<S_BufferData>> ModelMap;
	std::unordered_map<const FMatrix4*, std::vector<S_BufferData>> UIRectMap;
	std::unordered_map<Material*, std::vector<std::vector<S_BufferData>>> MaterialMap;
	std::vector<S_BufferData> CameraData;
	std::vector<S_BufferData> AspectRatioData;
	std::vector<S_BufferData> LightsData;
	std::vector<S_BufferData> NumberOfLightsData;

	VkDescriptorPool_T* DescriptorPool = nullptr;

	std::unordered_map<const FMatrix4*, std::vector<VkDescriptorSet_T*>> DescriptorSetsMap;
	std::unordered_map<std::string, VkDescriptorSetLayout_T*> DescriptorLayoutsByShader;

	std::unordered_map<const FMatrix4*, std::vector<VkDescriptorSet_T*>> UIDescriptorSetsMap;
	std::unordered_map<std::string, VkDescriptorSetLayout_T*> UIDescriptorSetLayoutByShader;

	std::unordered_map<S_Texture*, S_TextureData> TextureDataMap;
	std::unordered_map<S_CubeSampler*, S_TextureData> SkyboxDataMap;

	VulkanManager* Manager = nullptr;

	void CreateDescriptorSetLayoutFromGenericMaterial(std::string shader, Material* material);
	void CreateDescriptorSetLayoutFromVulkanMaterial(std::string shader, M_VulkanMaterial* material);

	void CreateUIDescriptorSetLayoutFromGenericMaterial(std::string shader, M_UI_Material* material);
	void CreateUIDescriptorSetLayoutFromVulkanMaterial(std::string shader, M_UI_VulkanMaterial* material);
};
#endif

