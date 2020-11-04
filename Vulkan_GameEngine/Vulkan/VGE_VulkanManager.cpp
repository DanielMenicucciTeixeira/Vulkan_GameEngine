#include "VGE_VulkanManager.h"
#include "SDL/VGE_SDLManager.h"
#include "VGE_VulkanValidationLayers.h"
#include "VGE_VulkanDebugMessenger.h"
#include "VGE_VulkanQueueManager.h"
#include "VGE_VulkanWindowManager.h"
#include"SDL/VGE_SDLManager.h"
#include"VGE_VulkanSwapChainManager.h"

#include <vulkan/vulkan.h>
#include <SDL.h>
#include<SDL_vulkan.h>

#include <iostream>
#include <set>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>



VGE_VulkanManager::VGE_VulkanManager()
{
}

VGE_VulkanManager::VGE_VulkanManager(VGE_SDLManager* sdlManager)
{
    SDLManager = sdlManager;
}

VGE_VulkanManager::~VGE_VulkanManager()
{
    if (QueueManager) delete(QueueManager);
    if (WindowManager) delete(WindowManager);
    //if (SwapChainManager) delete(SwapChainManager);
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

SDL_Window* VGE_VulkanManager::GetWindow()
{
    return WindowManager->GetWindow();
}

VkSurfaceKHR_T* VGE_VulkanManager::GetSurface()
{
    return WindowManager->GetSurface();
}

void VGE_VulkanManager::Initialize()
{   
    ValidationLayers = new VGE_VulkanValidationLayers();
    QueueManager = new VGE_VulkanQueueManager(this);
    WindowManager = new VGE_VulkanWindowManager(SDLManager->GetWindow());
    SwapChainManager = new VGE_VulkanSwapChainManager(this);

    if(ValidationLayers->IsEnabled()) DebugMessenger = new VGE_VulkanDebugMessenger(ValidationLayers);
    CreateInstance();
    if(DebugMessenger) DebugMessenger->SetUp(Instance);
    WindowManager->CreateSurface(Instance);
    PickPhysicalDevice();
    CreateLogicalDevice();
    SwapChainManager->CreateSwapChain(PhysicalDevice, LogicalDevice);
}

void VGE_VulkanManager::MainLoop()
{
}

void VGE_VulkanManager::Cleanup()
{
    vkDestroySwapchainKHR(LogicalDevice, SwapChainManager->GetSwapChain(), nullptr);
    vkDestroyDevice(LogicalDevice, nullptr);

    if (ValidationLayers->IsEnabled())
    {
        DebugMessenger->DestroyDebugUtilsMessengerEXT(Instance, nullptr);
    }

    vkDestroySurfaceKHR(Instance, WindowManager->GetSurface(), nullptr);
    vkDestroyInstance(Instance, nullptr);
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
    sdlExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(sdlExtensions.size());
    createInfo.ppEnabledExtensionNames = sdlExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    auto layerVector = ValidationLayers->GetVector();
    if (ValidationLayers->IsEnabled()) 
    {
        DebugMessenger->PopulateDebugMessengerCreateInfo(debugCreateInfo);
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

void VGE_VulkanManager::PickPhysicalDevice()
{
    PhysicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

    for (const auto& device : devices) 
    {
        if (IsPhysicalDeviceSuitable(device)) 
        {
            PhysicalDevice = device;
            break;
        }
    }

    if (PhysicalDevice == VK_NULL_HANDLE) 
    {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

bool VGE_VulkanManager::IsPhysicalDeviceSuitable(VkPhysicalDevice_T* device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return  deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
            QueueManager->FindQueueFamilies(device).IsComplete() &&
            SwapChainManager->CheckDeviceExtensionSupport(device) &&
            !SwapChainManager->QuerySwapChainSupport(device).Formats.empty() &&
            !SwapChainManager->QuerySwapChainSupport(device).PresentationModes.empty() &&
            deviceFeatures.geometryShader;
}

void VGE_VulkanManager::CreateLogicalDevice()
{
    QueueFamilyIndices indices = QueueManager->FindQueueFamilies(PhysicalDevice);

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

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(SwapChainManager->GetDeviceExtensions().size());
    createInfo.ppEnabledExtensionNames = SwapChainManager->GetDeviceExtensions().data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;
    auto layerVector = ValidationLayers->GetVector();
    if (ValidationLayers->IsEnabled()) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers->GetValidationLayerNames().size());
        createInfo.ppEnabledLayerNames = layerVector.data();
    }
    else 
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &LogicalDevice) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(LogicalDevice, indices.GraphicsFamily.value(), 0, &QueueManager->GetQueues()->GraphicsQueue);
    vkGetDeviceQueue(LogicalDevice, indices.PresentationFamily.value(), 0, &QueueManager->GetQueues()->PresentationQueue);
}
