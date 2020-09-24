#include "VGE_VulkanManager.h"
#include "SDL/VGE_SDLManager.h"
#include "VGE_VulkanValidationLayers.h"
#include "VGE_VulkanDebugMessenger.h"

#include <vulkan/vulkan.h>
#include <iostream>

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
    ValidationLayers = new VGE_VulkanValidationLayers();
    if(ValidationLayers->IsEnabled()) DebugMessenger = new VGE_VulkanDebugMessenger(ValidationLayers);
    CreateInstance();
    if(DebugMessenger) DebugMessenger->SetUp(Instance);
}

void VGE_VulkanManager::MainLoop()
{
}

void VGE_VulkanManager::Cleanup()
{
    printf("Cleanup Initiated\n");
    if (ValidationLayers->IsEnabled())
    {
        DebugMessenger->DestroyDebugUtilsMessengerEXT(Instance, nullptr);
        printf("DebugMessengerCleaned\n");
    }

    vkDestroyInstance(Instance, nullptr);
    printf("Cleanup Finished\n");
}

void VGE_VulkanManager::CreateInstance(const char* applicationName, const char* engineName/*, int appVersion[3], int engineVersion[3], int apiVersion[3]*/)
{

    if (ValidationLayers->IsEnabled() && !ValidationLayers->CheckValidationLayerSupport()) 
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    std::vector<const char*> sdlExtensions;
    SDLManager->GetVulkanExtensions(sdlExtensions);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(sdlExtensions.size());
    createInfo.ppEnabledExtensionNames = sdlExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    auto layerVector = ValidationLayers->GetVector();
    if (ValidationLayers->IsEnabled()) 
    {
        debugCreateInfo = DebugMessenger->PopulateDebugMessengerCreateInfo();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

        createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers->GetValidationLayerNames().size());
        createInfo.ppEnabledLayerNames = layerVector.data();
    }
    else 
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &Instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}
