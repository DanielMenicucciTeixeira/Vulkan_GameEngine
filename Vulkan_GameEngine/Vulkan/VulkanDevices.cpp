#include "VulkanDevices.h"
#include "VulkanManager.h"
#include "VulkanDebugger.h"

#include <vulkan/vulkan.h>
#include <set>
#include <string>
#include <iostream>

VulkanDevices::VulkanDevices(VulkanManager* manager)
{
	Manager = manager;
    DeviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

void VulkanDevices::PickPhysicalDevice()
{
    Physical = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(Manager->GetInstance(), &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(Manager->GetInstance(), &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (IsPhysicalDeviceSuitable(device))
        {
            Physical = device;
            break;
        }
    }

    if (Physical == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

bool VulkanDevices::IsPhysicalDeviceSuitable(VkPhysicalDevice_T* device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return  deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        Manager->FindQueueFamilies(device).IsComplete() &&
        CheckDeviceExtensionSupport(device) &&
        !QuerySwapChainSupport(device).Formats.empty() &&
        !QuerySwapChainSupport(device).PresentationModes.empty() &&
        deviceFeatures.samplerAnisotropy &&
        deviceFeatures.geometryShader;
}

void VulkanDevices::CreateLogicalDevice()
{
    QueueFamilyIndices indices = Manager->FindQueueFamilies(Physical);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentationFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = DeviceExtensions.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    if (Manager->GetDebugger()->IsEnabled())
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(Manager->GetDebugger()->GetValidationLayerNames()->size());
        createInfo.ppEnabledLayerNames = Manager->GetDebugger()->GetValidationLayerNames()->data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(Physical, &createInfo, nullptr, &Logical) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(Logical, indices.GraphicsFamily.value(), 0, &Manager->GetQueues()->GraphicsQueue);
    vkGetDeviceQueue(Logical, indices.PresentationFamily.value(), 0, &Manager->GetQueues()->PresentationQueue);
}

bool VulkanDevices::CheckDeviceExtensionSupport(VkPhysicalDevice_T* device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

    for (const auto& extension : availableExtensions) requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

SwapchainSupportDetails VulkanDevices::GetSwapchainSupportDetails()
{
    if (!Physical)
    {
        throw std::runtime_error("No Physical Device picked yet!");
    }
	return QuerySwapChainSupport(Physical);
}

SwapchainSupportDetails VulkanDevices::QuerySwapChainSupport(VkPhysicalDevice_T* device)
{
    SwapchainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Manager->GetSurface(), details.Capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, Manager->GetSurface(), &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.Formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, Manager->GetSurface(), &formatCount, details.Formats.data());
    }

    uint32_t presentationModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, Manager->GetSurface(), &presentationModeCount, nullptr);

    if (presentationModeCount != 0)
    {
        details.PresentationModes.resize(presentationModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, Manager->GetSurface(), &presentationModeCount, details.PresentationModes.data());
    }

    return details;
}
