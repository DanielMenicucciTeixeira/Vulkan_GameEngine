#ifndef VGE_VULKANSWAPCHAINMANAGER_H
#define VGE_VULKANSWAPCHAINMANAGER_H

#include <vector>

enum VkPresentModeKHR;

class VGE_VulkanManager;

struct VkPhysicalDevice_T;
struct VkDevice_T;
struct VkSurfaceCapabilitiesKHR;
struct VkSurfaceFormatKHR;
struct VkExtent2D;
struct VkSwapchainKHR_T;

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR* Capabilities = InitializeCapabilities();
	std::vector<VkSurfaceFormatKHR*> Formats;
	std::vector<VkPresentModeKHR> PresentationModes;

	//This function makes sure Capabilities is not a nullptr, this is necessary since Capabilites cannot be properly initilized in the header while using forward declarations.
	VkSurfaceCapabilitiesKHR* InitializeCapabilities();
	
	//This function makes sure each Format in the vector is not a nullptr, this is necessary since Formats cannot be properly initilized in the header while using forward declarations.
	void InitializeFormats();
	
};

class VGE_VulkanSwapChainManager
{
public:
	//----------Constructors------------\\
	
	VGE_VulkanSwapChainManager(VGE_VulkanManager* vulkanManager);

	//----------Destructors------------\\

	~VGE_VulkanSwapChainManager();

	//----------Functions------------\\
	
public:
	bool CheckDeviceExtensionSupport(VkPhysicalDevice_T* device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice_T* device);
	
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR*> availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities);

	void CreateSwapChain(VkPhysicalDevice_T* physicalDevice, VkDevice_T* logicalDevice);

	inline std::vector<const char*> GetDeviceExtensions() { return DeviceExtensions; }
	inline VkSwapchainKHR_T* GetSwapChain() { return SwapChain; }

	//----------Variables------------\\

protected:
	VkSwapchainKHR_T* SwapChain = nullptr;

	std::vector<const char*> DeviceExtensions;

	VGE_VulkanManager* VulkanManager = nullptr;
};
#endif
