#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include <vector>
#include <string>

class VulkanManager;

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

typedef unsigned int VkFlags;
typedef VkFlags VkImageAspectFlags;
typedef VkFlags VkFormatFeatureFlags;
typedef VkFlags VkImageUsageFlags;
typedef VkFlags VkMemoryPropertyFlags;

class VulkanSwapchain
{
public:
	VulkanSwapchain(VulkanManager* manager);

//Functions
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities);

	void CreateSwapChain();
	void RecreateSwapChain();

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
	void CreateTextureImageView();
	void CreateTextureSampler();

	void CleanUpSwapChain();

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	void TransitionImageLayout(VkImage_T* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	bool HasStencilComponent(VkFormat format);
	void CopyBufferToImage(VkBuffer_T* buffer, VkImage_T* image, unsigned int width, unsigned int height);

//Variables
protected:
	VkSwapchainKHR_T* SwapChain = nullptr;
	std::vector<const char*> DeviceExtensions;
	std::vector<VkImage_T*> SwapChainImages;
	std::vector<VkFramebuffer_T*> SwapChainFramebuffers;
	VkFormat SwapChainImageFormat;
	VkExtent2D* SwapChainExtent = nullptr;
	std::vector<VkImageView_T*> SwapChainImageViews;
	VkRenderPass_T* RenderPass = nullptr;
	VkImage_T* DepthImage;
	VkDeviceMemory_T* DepthImageMemory;
	VkImageView_T* DepthImageView;
	std::vector<VkBuffer_T*> UniformBuffers;
	std::vector<VkDeviceMemory_T*> UniformBuffersMemory;
	VkDescriptorSetLayout_T* DescriptorSetLayout = nullptr;
	VkDescriptorPool_T* DescriptorPool = nullptr;
	std::vector<VkDescriptorSet_T*> DescriptorSets;
	VkImage_T* TextureImage = nullptr;
	VkImageView_T* TextureImageView = nullptr;
	VkDeviceMemory_T* TextureImageMemory = nullptr;
	VkSampler_T* TextureSampler = nullptr;

	VulkanManager* Manager = nullptr;

	const std::string TEXTURE_PATH = "Textures/viking_room.png";
};
#endif

