#include "VGE_VulkanValidationLayers.h"

#include <vulkan/vulkan.h>

VGE_VulkanValidationLayers::VGE_VulkanValidationLayers()
{
#ifdef NDEBUG
    EnableValidationLayers = false;
#else
    EnableValidationLayers = true;
#endif
}

VGE_VulkanValidationLayers::~VGE_VulkanValidationLayers()
{
}

bool VGE_VulkanValidationLayers::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : ValidationLayerNames)
    {
        for (const char* layerName : ValidationLayerNames)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }
    }

    return true;
}

const char* const* VGE_VulkanValidationLayers::GetData()
{
    std::vector<const char*> layerVector;
    for (const auto& layerName : ValidationLayerNames) layerVector.push_back(layerName);
    return layerVector.data();
}

std::vector<const char*> VGE_VulkanValidationLayers::GetVector()
{
    std::vector<const char*> layerVector;
    for (const auto& layerName : ValidationLayerNames) layerVector.push_back(layerName);
    return layerVector;
}

VkResult VGE_VulkanValidationLayers::CreateDebugUtilsMessengerEXT(VkInstance_T*& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT_T** pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
