#include "VGE_VulkanManager.h"
#include "SDL/VGE_SDLManager.h"

#include <vulkan\vulkan.h>
#include <iostream>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>



VGE_VulkanManager::VGE_VulkanManager()
{
}

VGE_VulkanManager::~VGE_VulkanManager()
{
    //if(Instance) delete(Instance);
}

void VGE_VulkanManager::run()
{
    Initialize();
    MainLoop();
    Cleanup();
}

void VGE_VulkanManager::TestMe()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";
}

void VGE_VulkanManager::Initialize()
{
    CreateInstance();
}

void VGE_VulkanManager::MainLoop()
{
}

void VGE_VulkanManager::Cleanup()
{
    vkDestroyInstance(Instance, nullptr);
}

void VGE_VulkanManager::CreateInstance(unsigned appVersion[3], unsigned engineVersion[3], unsigned apiVersion[3], const char* applicationName, const char* engineName)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = (uint32_t)appVersion;
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = (uint32_t)engineVersion;
    appInfo.apiVersion = (uint32_t)apiVersion;
}

void VGE_VulkanManager::CreateInstance(const char* applicationName, const char* engineName)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    std::vector<const char*> sdlExtensions;
    SDLManager->GetVulkanExtensions(sdlExtensions);
    createInfo.enabledExtensionCount = sdlExtensions.size();
    createInfo.ppEnabledExtensionNames = sdlExtensions.data();
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &Instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}
