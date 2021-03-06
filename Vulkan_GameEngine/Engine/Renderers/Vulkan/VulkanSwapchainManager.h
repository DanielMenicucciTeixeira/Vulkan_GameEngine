#ifndef VULKANSWAPCHAINMANAGER_H
#define VULKANSWAPCHAINMANAGER_H

#include <vector>
#include <string>
#include <unordered_map>

class VulkanManager;
class FMatrix4;

enum VkPresentModeKHR;
enum VkFormat;
enum VkImageTiling;
enum VkImageLayout;

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
struct VkSampler_T;
struct UniformBufferObject;
struct UniformCameraObject;
struct S_Texture;

typedef unsigned int VkFlags;
typedef VkFlags VkImageAspectFlags;
typedef VkFlags VkFormatFeatureFlags;
typedef VkFlags VkImageUsageFlags;
typedef VkFlags VkMemoryPropertyFlags;

struct S_BufferData
{
	VkBuffer_T* Buffer;
	VkDeviceMemory_T* Memory;
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
	VkImageView_T* CreateImageView(VkImage_T* image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateRenderPass();
	void CreateImage(unsigned int width, unsigned int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage_T*& image, VkDeviceMemory_T*& imageMemory);
	void CreateDepthResources();
	void CreateFramebuffers();
	void CreateUniformBuffers();
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateTextureImage();
	void CreateTextureSampler(VkSampler_T*& sampler);

	void RecreationCleanUp();
	void FinalCleanUp();

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	void TransitionImageLayout(VkImage_T* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	bool HasStencilComponent(VkFormat format);
	void CopyBufferToImage(VkBuffer_T* buffer, VkImage_T* image, unsigned int width, unsigned int height);

	inline VkExtent2D* GetExtent() { return Extent; }
	inline VkRenderPass_T* GetRenderPass() { return RenderPass; }
	inline std::vector<VkFramebuffer_T*> GetFramebuffers() { return Framebuffers; }
	inline std::unordered_map<FMatrix4*, std::vector<VkDescriptorSet_T*>> GetDescriptorSetsMap() { return DescriptorSetsMap; }
	inline std::vector<VkImage_T*> GetImages() { return Images; }
	inline VkSwapchainKHR_T* GetSwapchain() { return Swapchain; }
	inline VkDescriptorSetLayout_T* GetDescriptorSetLayout() { return DescriptorSetLayout; }

	void UpdateBuffers(unsigned int currentImageIndex);

//Variables
protected:
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

	std::unordered_map<FMatrix4*, std::vector<S_BufferData>> ModelMap;
	std::unordered_map<FMatrix4*, std::vector<S_BufferData>> MaterialMap;
	std::vector<S_BufferData> CameraData;
	std::vector<S_BufferData> LightsData;
	std::vector<S_BufferData> NumberOfLightsData;

	VkDescriptorPool_T* DescriptorPool = nullptr;
	std::unordered_map<FMatrix4*, std::vector<VkDescriptorSet_T*>> DescriptorSetsMap;
	VkDescriptorSetLayout_T* DescriptorSetLayout;

	std::unordered_map<S_Texture*, S_TextureData> TextureDataMap;

	VulkanManager* Manager = nullptr;

	const std::string TEXTURE_PATH = "Textures/viking_room.png";
};
#endif

