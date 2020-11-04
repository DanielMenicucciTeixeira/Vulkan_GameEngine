#include "VGE_VulkanSwapChainManager.h"
#include "VGE_VulkanManager.h"
#include "VGE_VulkanQueueManager.h"

#include <vulkan/vulkan.h>
#include<SDL_vulkan.h>
#include<SDL.h>

#include<set>
#include<string>
#include <algorithm> // Necessary for UINT32_MAX
#include<iostream>

VGE_VulkanSwapChainManager::VGE_VulkanSwapChainManager(VGE_VulkanManager* vulkanManager)
{
    VulkanManager = vulkanManager;

	DeviceExtensions = 
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
}

VGE_VulkanSwapChainManager::~VGE_VulkanSwapChainManager()
{
    if (VulkanManager) delete(VulkanManager);
}

bool VGE_VulkanSwapChainManager::CheckDeviceExtensionSupport(VkPhysicalDevice_T* device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

    for (const auto& extension : availableExtensions) requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

SwapChainSupportDetails VGE_VulkanSwapChainManager::QuerySwapChainSupport(VkPhysicalDevice_T* device)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanManager->GetSurface(), details.Capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanManager->GetSurface(), &formatCount, nullptr);

    if (formatCount != 0) 
    {
        details.Formats.resize(formatCount);
        details.InitializeFormats();
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanManager->GetSurface(), &formatCount, *details.Formats.data());
    }

    uint32_t presentationModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanManager->GetSurface(), &presentationModeCount, nullptr);

    if (presentationModeCount != 0) 
    {
        details.PresentationModes.resize(presentationModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanManager->GetSurface(), &presentationModeCount, details.PresentationModes.data());
    }

    return details;
}

VkSurfaceFormatKHR VGE_VulkanSwapChainManager::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR*> availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat->format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return *availableFormat;
        }
    }

    return *availableFormats[0];
}

VkPresentModeKHR VGE_VulkanSwapChainManager::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) 
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VGE_VulkanSwapChainManager::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities)
{
    if (capabilities->currentExtent.width != UINT32_MAX) 
    {
        return capabilities->currentExtent;
    }
    else 
    {
        SDL_Window* window = VulkanManager->GetWindow();
        int width, height;
        SDL_Vulkan_GetDrawableSize(window, &width, &height);

        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        actualExtent.width = std::max(capabilities->minImageExtent.width, std::min(capabilities->maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities->minImageExtent.height, std::min(capabilities->maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

void VGE_VulkanSwapChainManager::CreateSwapChain(VkPhysicalDevice_T* physicalDevice, VkDevice_T* logicalDevice)
{
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
    VkPresentModeKHR presentationMode = ChooseSwapPresentMode(swapChainSupport.PresentationModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

    uint32_t imageCount = swapChainSupport.Capabilities->minImageCount + 1;
    if (swapChainSupport.Capabilities->maxImageCount > 0 && imageCount > swapChainSupport.Capabilities->maxImageCount) imageCount = swapChainSupport.Capabilities->maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = VulkanManager->GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VulkanManager->GetQueueManager()->FindQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentationFamily.value() };

    if (indices.GraphicsFamily != indices.PresentationFamily) 
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.Capabilities->currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentationMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

   if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &SwapChain) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create swap chain!");
    }
}

VkSurfaceCapabilitiesKHR* SwapChainSupportDetails::InitializeCapabilities()
{
    return new VkSurfaceCapabilitiesKHR();
}

void SwapChainSupportDetails::InitializeFormats()
{
    for (auto& format : Formats) format = new VkSurfaceFormatKHR();
}


