#include "VGE_VulkanDebugMessenger.h"
#include "VGE_VulkanValidationLayers.h"

#include<vulkan/vulkan.h>
#include <iostream>

VGE_VulkanDebugMessenger::VGE_VulkanDebugMessenger(VGE_VulkanValidationLayers* validationLayers)
{
	ValidationLayers = validationLayers;
}

VGE_VulkanDebugMessenger::~VGE_VulkanDebugMessenger()
{
	//if (ValidationLayers) delete(ValidationLayers);
}

void VGE_VulkanDebugMessenger::DestroyDebugUtilsMessengerEXT(VkInstance_T* instance, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        func(instance, DebugMessenger, pAllocator);
    }
}

//This function will not be used anywhere else and is declared here, rather the in the header, to avoid some complications in forward declaring it.
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT VGE_VulkanDebugMessenger::PopulateDebugMessengerCreateInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    return createInfo;
}

void VGE_VulkanDebugMessenger::SetUp(VkInstance_T* instance)
{
    if (!ValidationLayers->IsEnabled()) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo = PopulateDebugMessengerCreateInfo();

    if (ValidationLayers->CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &DebugMessenger) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
