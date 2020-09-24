#ifndef VGE_VULAKDEBUGMESSENGER_H
#define VGE_VULKANDEBUGMESSENGER_H

class VGE_VulkanValidationLayers;
struct VkDebugUtilsMessengerCreateInfoEXT;
struct VkInstance_T;
struct VkAllocationCallbacks;
struct VkDebugUtilsMessengerEXT_T;

class VGE_VulkanDebugMessenger
{
public:
	//-----Constructors-----\\

	VGE_VulkanDebugMessenger(VGE_VulkanValidationLayers* validationLayers = nullptr);

	//-----Destructors-----\\

	~VGE_VulkanDebugMessenger();

	void DestroyDebugUtilsMessengerEXT(VkInstance_T* instance, const VkAllocationCallbacks* pAllocator);

	//------Functions------\\

public:
	void SetUp(VkInstance_T* instance);

	inline VkDebugUtilsMessengerEXT_T* GetDebugMessenger() { return DebugMessenger; }
	VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();

	//------Variables-------\\

protected:
	VGE_VulkanValidationLayers* ValidationLayers = nullptr;
	VkDebugUtilsMessengerEXT_T* DebugMessenger = nullptr;
};

#endif