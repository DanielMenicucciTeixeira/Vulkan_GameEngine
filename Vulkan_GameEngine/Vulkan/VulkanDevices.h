#ifndef VULKANDEVICES_H
#define VULKANDEVICES_H

#include <vector>

enum VkPresentModeKHR;

class VulkanManager;
struct VkPhysicalDevice_T;
struct QueueFamilyIndices;
struct VkDevice_T;
struct VkSurfaceCapabilitiesKHR;
struct VkSurfaceFormatKHR;
struct SwapchainSupportDetails;

//-----------------------------------------------------

class VulkanDevices
{
public:
	VulkanDevices(VulkanManager* manager);

	void PickPhysicalDevice();
	bool IsPhysicalDeviceSuitable(VkPhysicalDevice_T* device);
	void CreateLogicalDevice();

	inline VkPhysicalDevice_T* GetPhysicalDevice() { return Physical; }
	inline VkDevice_T* GetLogicalDevice() { return Logical; }
	SwapchainSupportDetails GetSwapChainSupportDetails();

protected:
	SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice_T* device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice_T* device);

	VulkanManager* Manager = nullptr;
	VkPhysicalDevice_T* Physical;
	VkDevice_T* Logical;
	std::vector<const char*> DeviceExtensions;
};
#endif

