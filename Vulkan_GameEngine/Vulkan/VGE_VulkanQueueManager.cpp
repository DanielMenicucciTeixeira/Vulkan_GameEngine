#include "VGE_VulkanQueueManager.h"
#include "VGE_VulkanManager.h"

#include <vulkan/vulkan.h>
#include <vector>

VGE_VulkanQueueManager::VGE_VulkanQueueManager(VGE_VulkanManager* vulkanManager)
{
	VulkanManager = vulkanManager;
}

VGE_VulkanQueueManager::~VGE_VulkanQueueManager()
{
}

QueueFamilyIndices VGE_VulkanQueueManager::FindQueueFamilies(VkPhysicalDevice_T* device)
{
	QueueFamilyIndices indices;
	
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.GraphicsFamily = i;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VulkanManager->GetSurface(), &presentSupport);

		if (presentSupport) {
			indices.PresentationFamily = i;
		}


		if (indices.IsComplete()) break;
		i++;
	}

	return indices;
}

/*bool VGE_VulkanQueueManager::IsComplete(QueueFamilyIndices indices)
{
	return indices.GraphicsFamily.has_value();
}*/
