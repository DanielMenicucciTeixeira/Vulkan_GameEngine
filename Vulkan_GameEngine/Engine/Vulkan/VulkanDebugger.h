#ifndef VULKANDEBUGGER_H
#define VULKANDEBUGGER_H

#include <vector>

enum VkResult;

class VulkanManager;
struct VkDebugUtilsMessengerCreateInfoEXT;
struct VkInstance_T;
struct VkAllocationCallbacks;
struct VkDebugUtilsMessengerEXT_T;
struct VkDebugUtilsMessengerCreateInfoEXT;

class VulkanDebugger
{
public:
	VulkanDebugger(VulkanManager* manager);

	bool CheckValidationLayerSupport();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance_T* instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT_T** pDebugMessenger);
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void SetUpDebugMessenger();
	void DestroyDebugUtilsMessengerEXT(VkInstance_T* instance, const VkAllocationCallbacks* pAllocator);

	
	inline bool IsEnabled() { return Enabled; }
	const std::vector<const char*>* const GetValidationLayerNames();

protected:
	VulkanManager* Manager = nullptr;
	bool Enabled = false;
	VkDebugUtilsMessengerEXT_T* DebugMessenger = nullptr;

	std::vector<const char*> ValidationLayerNames =
	{
		"VK_LAYER_KHRONOS_validation"
	};
};
#endif

