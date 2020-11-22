#include "VulkanManager.h"

//Basic includes
#include <iostream>
#include <fstream>

///VulkanRenderer includes:
#include "VulkanDebugger.h"
#include "VulkanDevices.h"
#include "VulkanPipeline.h"
#include "VulkanSwapchain.h"

//Vulkan API includes
#include <vulkan/vulkan.h>

//SDL includes
#include "SDL/VGE_SDLManager.h"
#include <SDL_vulkan.h>

VulkanManager::VulkanManager(VGE_SDLManager* windowManager)
{
	Debugger = new VulkanDebugger(this);
	Devices = new VulkanDevices(this);
	GraphicsPipeline = new VulkanPipeline(this);
	Swapchain = new VulkanSwapchain(this);
    Queues = new QueueStruct();
	WindowManager = windowManager;
    Window = WindowManager->GetWindow();

    Initialize();
}

VulkanManager::~VulkanManager()
{
    CleanUp();

	if (Debugger) delete(Debugger);
	if (Devices) delete(Devices);
	if (GraphicsPipeline) delete(GraphicsPipeline);
	if (Swapchain) delete(Swapchain);
    if (Queues) delete(Queues);
}

void VulkanManager::Run()
{
}

void VulkanManager::FramebufferResizeCallback()
{
}

VkDevice_T* VulkanManager::GetLogicalDevice()
{
    return Devices->GetLogicalDevice();
}

VkPhysicalDevice_T* VulkanManager::GetPhysicalDevice()
{
    return Devices->GetPhysicalDevice();
}

SwapchainSupportDetails VulkanManager::GetSwapchainSupportDetails()
{
    return Devices->GetSwapChainSupportDetails();
}

QueueFamilyIndices VulkanManager::FindQueueFamilies(VkPhysicalDevice_T* physicalDevice)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.GraphicsFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, Surface, &presentSupport);

        if (presentSupport) {
            indices.PresentationFamily = i;
        }


        if (indices.IsComplete()) break;
        i++;
    }

    return indices;
}

unsigned int VulkanManager::FindMemoryType(unsigned int typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(Devices->GetPhysicalDevice(), &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type!");

    return 0;
}

void VulkanManager::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer_T*& buffer, VkDeviceMemory_T*& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Devices->GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Devices->GetLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Devices->GetLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(Devices->GetLogicalDevice(), buffer, bufferMemory, 0);
}

void VulkanManager::CreateCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(Devices->GetPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(Devices->GetLogicalDevice(), &poolInfo, nullptr, &CommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

VkCommandBuffer_T* VulkanManager::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = CommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(Devices->GetLogicalDevice(), &allocInfo, &commandBuffer);
    /*
      TODO It should be noted that in a real world application, you're not supposed to actually call vkAllocateMemory for every individual buffer.
      The maximum number of simultaneous memory allocations is limited by the maxMemoryAllocationCount physical device limit, which may be as low as
      4096 even on high end hardware like an NVIDIA GTX 1080. The right way to allocate memory for a large number of objects at the same time is to
      create a custom allocator that splits up a single allocation among many different objects by using the offset parameters that we've seen in many
      functions.
      You can either implement such an allocator yourself, or use the VulkanMemoryAllocator library provided by the GPUOpen initiative.
      However, for this tutorial it's okay to use a separate allocation for every resource, because we won't come close to hitting any of these limits for now.
  */
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanManager::EndSingleTimeCommands(VkCommandBuffer_T* commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Queues->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Queues->GraphicsQueue);

    vkFreeCommandBuffers(Devices->GetLogicalDevice(), CommandPool, 1, &commandBuffer);
}

void VulkanManager::Initialize()
{
    CreateInstance();
    Debugger->SetUpDebugMessenger();
    CreateSurface();
    Devices->PickPhysicalDevice();
    Devices->CreateLogicalDevice();
    Swapchain->CreateSwapChain();
    Swapchain->CreateImageViews();
    Swapchain->CreateRenderPass();
    Swapchain->CreateDescriptorSetLayout();
}

void VulkanManager::CleanUp()
{
}

SDL_Window* VulkanManager::CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY)
{
	return WindowManager->GetWindow();
}

void VulkanManager::CreateSurface()
{
    if (SDL_Vulkan_CreateSurface(Window, Instance, &Surface) != SDL_TRUE)
    {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void VulkanManager::Render(SDL_Window** windowArray, unsigned int numberOfWindows, unsigned int arrayOffset)
{
}

void VulkanManager::CreateInstance(const char* applicationName, const char* engineName, unsigned int* appVersion)
{
    if (Debugger->IsEnabled() && !Debugger->CheckValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName;
    if(!appVersion) appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    else appInfo.applicationVersion = VK_MAKE_VERSION(*appVersion, *(appVersion + 1), *(appVersion + 2));
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    std::vector<const char*> sdlExtensions;
    WindowManager->GetVulkanExtensions(sdlExtensions);
    sdlExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(sdlExtensions.size());
    createInfo.ppEnabledExtensionNames = sdlExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (Debugger->IsEnabled())
    {
        Debugger->PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

        createInfo.enabledLayerCount = static_cast<uint32_t>(Debugger->GetValidationLayerNames().size());
        createInfo.ppEnabledLayerNames = Debugger->GetValidationLayerNames().data();
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

VkSurfaceCapabilitiesKHR* SwapchainSupportDetails::InitializeCapabilities02()
{
    return new VkSurfaceCapabilitiesKHR();
}
