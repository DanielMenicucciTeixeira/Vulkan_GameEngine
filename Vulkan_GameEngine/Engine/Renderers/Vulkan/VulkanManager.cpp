#include "VulkanManager.h"

//Basic includes
#include <iostream>
#include <fstream>

///VulkanRenderer includes:
#include "VulkanDebugger.h"
#include "VulkanDevices.h"
#include "VulkanPipelineManager.h"
#include "VulkanSwapchainManager.h"
#include "Vertex.h"

//Vulkan API includes
#include <vulkan/vulkan.h>

//SDL includes
#include "SDL/SDLManager.h"
#include <SDL_vulkan.h>
#include <SDL.h>

//ExtraIncludes
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Renderers/RenderObject.h"
#include "Renderers/RenderInitializationData.h"

VulkanManager::VulkanManager() : Renderer()
{
    Type = VULKAN;
	Debugger = new VulkanDebugger(this);
	Devices = new VulkanDevices(this);
	GraphicsPipelineManager = new VulkanPipelineManager(this);
	SwapchainManager = new VulkanSwapchainManager(this);
    Queues = new QueueStruct();
}

VulkanManager::~VulkanManager()
{
	if (Debugger) delete(Debugger);
	if (Devices) delete(Devices);
	if (GraphicsPipelineManager) delete(GraphicsPipelineManager);
	if (SwapchainManager) delete(SwapchainManager);
    if (Queues) delete(Queues);
}

void VulkanManager::FramebufferResizeCallback()
{
    FramebufferResized = true;
}

void VulkanManager::UpdateWithNewObjects()
{
    RecreateSwapchain();
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
    return Devices->GetSwapchainSupportDetails();
}

VkExtent2D* VulkanManager::GetSwapchainExtent()
{
    return SwapchainManager->GetExtent();
}

VkDescriptorSetLayout_T* VulkanManager::GetDescriptorSetLayout()
{
    return SwapchainManager->GetDescriptorSetLayout();
}

VkRenderPass_T* VulkanManager::GetRenderPass()
{
    return SwapchainManager->GetRenderPass();
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

void VulkanManager::RecreateSwapchain()
{
    int width = 0;
    int height = 0;

    do
    {
        SDL_GetWindowSize(Window, &width, &height);
        SDL_WaitEvent(nullptr);
    } while (width == 0 || height == 0);

    vkDeviceWaitIdle(Devices->GetLogicalDevice());
    SwapchainManager->RecreationCleanUp();
    vkFreeCommandBuffers(Devices->GetLogicalDevice(), CommandPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());

    SwapchainManager->CreateSwapChain();
    SwapchainManager->CreateImageViews();
    SwapchainManager->CreateRenderPass();
    SwapchainManager->CreateDepthResources();
    SwapchainManager->CreateFramebuffers();
    SwapchainManager->CreateUniformBuffers();
    SwapchainManager->CreateDescriptorPool();
    SwapchainManager->CreateDescriptorSets();
    CreateCommandBuffers();
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

void VulkanManager::CreateCommandBuffers()
{
    CommandBuffers.resize(SwapchainManager->GetFramebuffers().size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)CommandBuffers.size();

    if (vkAllocateCommandBuffers(Devices->GetLogicalDevice(), &allocInfo, CommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < CommandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(CommandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = SwapchainManager->GetRenderPass();
        renderPassInfo.framebuffer = SwapchainManager->GetFramebuffers()[i];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = *SwapchainManager->GetExtent();
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        VkDeviceSize offsets[] = { 0 };

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)GetSwapchainExtent()->width;
        viewport.height = (float)GetSwapchainExtent()->height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = *GetSwapchainExtent();

        vkCmdBeginRenderPass(CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdSetViewport(CommandBuffers[i], 0, 1, &viewport);
            vkCmdSetScissor(CommandBuffers[i], 0, 1, &scissor);
            for (const auto& mesh : InitializationData->MeshToMaterialMap)
            {
                vkCmdBindVertexBuffers(CommandBuffers[i], 0, 1, &MeshDataMap[mesh.first]->VertexBuffer, offsets);
                vkCmdBindIndexBuffer(CommandBuffers[i], MeshDataMap[mesh.first]->IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
                for (const auto& material : mesh.second)
                {
                    vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineManager->GetPipeline());
                    for (const auto& model : InitializationData->MaterialToModelMap[material])
                    {
                        vkCmdBindDescriptorSets(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineManager->GetPipelineLayout(), 0, 1, &SwapchainManager->GetDescriptorSetsMap()[model][i], 0, nullptr);
                        vkCmdDrawIndexed(CommandBuffers[i], static_cast<uint32_t>(mesh.first->Indices.size()), 1, 0, 0, 0);
                    }
                }
            }
        vkCmdEndRenderPass(CommandBuffers[i]);

        if (vkEndCommandBuffer(CommandBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void VulkanManager::CreateVertexBuffers()
{
    for (const auto& pair : InitializationData->MeshToMaterialMap)
    {
        VkDeviceSize bufferSize = sizeof(S_Vertex) * pair.first->Vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(Devices->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, pair.first->Vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(Devices->GetLogicalDevice(), stagingBufferMemory);

        MeshDataMap[pair.first] = new MeshDataStruct();
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MeshDataMap[pair.first]->VertexBuffer, MeshDataMap[pair.first]->VertexBufferMemory);
        CopyBuffer(stagingBuffer, MeshDataMap[pair.first]->VertexBuffer, bufferSize);

        vkDestroyBuffer(Devices->GetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(Devices->GetLogicalDevice(), stagingBufferMemory, nullptr);
    }
}

void VulkanManager::CreateIndexBuffers()
{
    for (const auto& pair : InitializationData->MeshToMaterialMap)
    {
        VkDeviceSize bufferSize = sizeof(unsigned int) * pair.first->Indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(Devices->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, pair.first->Indices.data(), (size_t)bufferSize);
        vkUnmapMemory(Devices->GetLogicalDevice(), stagingBufferMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MeshDataMap[pair.first]->IndexBuffer, MeshDataMap[pair.first]->IndexBufferMemory);

        CopyBuffer(stagingBuffer, MeshDataMap[pair.first]->IndexBuffer, bufferSize);

        vkDestroyBuffer(Devices->GetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(Devices->GetLogicalDevice(), stagingBufferMemory, nullptr);
    }
}

void VulkanManager::CopyBuffer(VkBuffer_T* srcBuffer, VkBuffer_T* dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer);
}

void VulkanManager::CreateSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    ImagesInFlight.resize(SwapchainManager->GetImages().size(), VK_NULL_HANDLE);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (
            vkCreateSemaphore(Devices->GetLogicalDevice(), &semaphoreInfo, nullptr, &ImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(Devices->GetLogicalDevice(), &semaphoreInfo, nullptr, &RenderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(Devices->GetLogicalDevice(), &fenceInfo, nullptr, &InFlightFences[i]) != VK_SUCCESS
            )
        {
            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
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

bool VulkanManager::Initialize(RenderInitializationData* initializationData)
{
    InitializationData = initializationData;

    CreateInstance();
    Debugger->SetUpDebugMessenger();
    CreateSurface();
    Devices->PickPhysicalDevice();
    Devices->CreateLogicalDevice();
    SwapchainManager->CreateSwapChain();
    SwapchainManager->CreateImageViews();
    SwapchainManager->CreateRenderPass();
    SwapchainManager->CreateDescriptorSetLayout();
    GraphicsPipelineManager->CreateGraphicsPipeline();
    CreateCommandPool();
    SwapchainManager->CreateDepthResources();
    SwapchainManager->CreateFramebuffers();
    SwapchainManager->CreateTextureImage();
    LoadModel();
    CreateVertexBuffers();
    CreateIndexBuffers();
    SwapchainManager->CreateUniformBuffers();
    SwapchainManager->CreateDescriptorPool();
    SwapchainManager->CreateDescriptorSets();
    CreateCommandBuffers();
    CreateSyncObjects();

    return true;
}

void VulkanManager::CleanUp()
{
    vkDeviceWaitIdle(Devices->GetLogicalDevice());

    SwapchainManager->FinalCleanUp();
    vkFreeCommandBuffers(Devices->GetLogicalDevice(), CommandPool, CommandBuffers.size(), CommandBuffers.data());
    vkDestroyPipeline(Devices->GetLogicalDevice(), GraphicsPipelineManager->GetPipeline(), nullptr);
    vkDestroyPipelineLayout(Devices->GetLogicalDevice(), GraphicsPipelineManager->GetPipelineLayout(), nullptr);
    for (const auto& semaphore : RenderFinishedSemaphores) vkDestroySemaphore(Devices->GetLogicalDevice(), semaphore, nullptr);
    for (const auto& semaphore : ImageAvailableSemaphores) vkDestroySemaphore(Devices->GetLogicalDevice(), semaphore, nullptr);
    for (const auto& fence : InFlightFences) vkDestroyFence(Devices->GetLogicalDevice(), fence, nullptr);
    vkDestroyCommandPool(Devices->GetLogicalDevice(), CommandPool, nullptr);
    for (const auto& meshData : MeshDataMap)
    {
        vkDestroyBuffer(Devices->GetLogicalDevice(), meshData.second->IndexBuffer, nullptr);
        vkFreeMemory(Devices->GetLogicalDevice(), meshData.second->IndexBufferMemory, nullptr);
        vkDestroyBuffer(Devices->GetLogicalDevice(), meshData.second->VertexBuffer, nullptr);
        vkFreeMemory(Devices->GetLogicalDevice(), meshData.second->VertexBufferMemory, nullptr);
        delete(meshData.second);
    }
    vkDestroyDevice(Devices->GetLogicalDevice(), nullptr);

    if (Debugger->IsEnabled()) Debugger->DestroyDebugUtilsMessengerEXT(Instance, nullptr);

    vkDestroySurfaceKHR(Instance, Surface, nullptr);
    vkDestroyInstance(Instance, nullptr);
}

SDL_Window* VulkanManager::CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY)
{
	return Window;
}

void VulkanManager::CreateSurface()
{
    Window = SDLManager::GetInstance()->GetSDLWindowByName();
    if (SDL_Vulkan_CreateSurface(Window, Instance, &Surface) != SDL_TRUE)
    {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void VulkanManager::Render(SDL_Window** windowArray, unsigned int numberOfWindows, unsigned int arrayOffset)
{
    vkWaitForFences(Devices->GetLogicalDevice(), 1, &InFlightFences[CurrentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult swapChainResult = vkAcquireNextImageKHR(Devices->GetLogicalDevice(), SwapchainManager->GetSwapchain(), UINT64_MAX, ImageAvailableSemaphores[CurrentFrame], VK_NULL_HANDLE, &imageIndex);
    if (swapChainResult == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapchain();
        return;
    }
    else if (swapChainResult != VK_SUCCESS && swapChainResult != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (ImagesInFlight[imageIndex] != VK_NULL_HANDLE) vkWaitForFences(Devices->GetLogicalDevice(), 1, &ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    // Mark the image as now being in use by this frame
    ImagesInFlight[imageIndex] = InFlightFences[CurrentFrame];

    SwapchainManager->UpdateBuffers(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { ImageAvailableSemaphores[CurrentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { RenderFinishedSemaphores[CurrentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(Devices->GetLogicalDevice(), 1, &InFlightFences[CurrentFrame]);
    if (vkQueueSubmit(Queues->GraphicsQueue, 1, &submitInfo, InFlightFences[CurrentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { SwapchainManager->GetSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    swapChainResult = vkQueuePresentKHR(Queues->PresentationQueue, &presentInfo);
    if (swapChainResult == VK_ERROR_OUT_OF_DATE_KHR || swapChainResult == VK_SUBOPTIMAL_KHR || FramebufferResized)
    {
        FramebufferResized = false;
        RecreateSwapchain();
    }
    else if (swapChainResult != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    CurrentFrame = (CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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
    std::vector<const char*> sdlExtensions = GetSDLExetensions();
    sdlExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(sdlExtensions.size());
    createInfo.ppEnabledExtensionNames = sdlExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (Debugger->IsEnabled())
    {
        Debugger->PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

        createInfo.enabledLayerCount = static_cast<uint32_t>(Debugger->GetValidationLayerNames()->size());
        createInfo.ppEnabledLayerNames = Debugger->GetValidationLayerNames()->data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &Instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }
}

std::vector<const char*> VulkanManager::GetSDLExetensions()
{
    std::vector<const char*> sdlExtensions;
    uint32_t extensionCount;
    if (SDL_Vulkan_GetInstanceExtensions(Window, &extensionCount, nullptr) == SDL_FALSE);
    sdlExtensions.resize(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(Window, &extensionCount, sdlExtensions.data());

    return sdlExtensions;
}

void VulkanManager::LoadModel()//TODO remove depricated code
{
   /* for (const auto& pair : InitializationData->MeshToMaterialMap)
    {
        Vertices = pair.first->Vertices;
        Indices = pair.first->Indices;
    }*/

    /*tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex = Vertex();

            vertex.Position =
            {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.TextureCoordinates =
            {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
                0
            };

            Vertices.push_back(vertex);
            Indices.push_back(Indices.size());
        }
    }*/
}

VkSurfaceCapabilitiesKHR* SwapchainSupportDetails::InitializeCapabilities()
{
    return new VkSurfaceCapabilitiesKHR();
}
