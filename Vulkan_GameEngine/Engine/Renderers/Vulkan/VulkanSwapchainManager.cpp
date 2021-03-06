#include "VulkanSwapchainManager.h"
#include "VulkanManager.h"
#include "Renderers/UniformBufferObject.h"
#include "Math/FVector3.h"
#include "Renderers/RenderObject.h"
#include "Renderers/RenderInitializationData.h"
#include "DebugLogger.h"
#include "TextureLoader.h"
#include "LevelGraph.h"

#include <vulkan/vulkan.h>
#include <SDL_vulkan.h>
#include<cmath>
#include <algorithm>
#include<fstream>
#include<array>
#include<chrono>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VulkanSwapchainManager::VulkanSwapchainManager(VulkanManager* manager)
{
	Manager = manager;
}

VulkanSwapchainManager::~VulkanSwapchainManager()
{
    if (Extent != nullptr) delete(Extent);
}

VkSurfaceFormatKHR VulkanSwapchainManager::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapchainManager::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchainManager::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities)
{
    if (capabilities->currentExtent.width != UINT32_MAX)
    {
        return capabilities->currentExtent;
    }
    else
    {
        SDL_Window* window = Manager->GetWindow();
        int width, height;
        SDL_Vulkan_GetDrawableSize(window, &width, &height);

        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        actualExtent.width = std::max(capabilities->minImageExtent.width, std::min(capabilities->maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities->minImageExtent.height, std::min(capabilities->maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

void VulkanSwapchainManager::CreateSwapChain()
{
    SwapchainSupportDetails swapChainSupport = Manager->GetSwapchainSupportDetails();

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
    VkPresentModeKHR presentationMode = ChooseSwapPresentMode(swapChainSupport.PresentationModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

    uint32_t imageCount = swapChainSupport.Capabilities->minImageCount + 1;
    if (swapChainSupport.Capabilities->maxImageCount > 0 && imageCount > swapChainSupport.Capabilities->maxImageCount) imageCount = swapChainSupport.Capabilities->maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = Manager->GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = Manager->FindQueueFamilies(Manager->GetPhysicalDevice());
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

    if (vkCreateSwapchainKHR(Manager->GetLogicalDevice(), &createInfo, nullptr, &Swapchain) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Swapchain!");
    }

    vkGetSwapchainImagesKHR(Manager->GetLogicalDevice(), Swapchain, &imageCount, nullptr);
    Images.resize(imageCount);
    vkGetSwapchainImagesKHR(Manager->GetLogicalDevice(), Swapchain, &imageCount, Images.data());

    ImageFormat = surfaceFormat.format;
    Extent = new VkExtent2D(extent);
}

void VulkanSwapchainManager::CreateImageViews()
{
    ImageViews.resize(Images.size());

    for (uint32_t i = 0; i < Images.size(); i++) {
        ImageViews[i] = CreateImageView(Images[i], ImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

VkImageView_T* VulkanSwapchainManager::CreateImageView(VkImage_T* image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(Manager->GetLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void VulkanSwapchainManager::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = ImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;


    if (vkCreateRenderPass(Manager->GetLogicalDevice(), &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void VulkanSwapchainManager::CreateImage(unsigned int width, unsigned int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage_T*& image, VkDeviceMemory_T*& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(Manager->GetLogicalDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Manager->GetLogicalDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Manager->FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Manager->GetLogicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(Manager->GetLogicalDevice(), image, imageMemory, 0);
}

void VulkanSwapchainManager::CreateDepthResources()
{
    VkFormat depthFormat = FindDepthFormat();

    CreateImage(Extent->width, Extent->height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, DepthImage, DepthImageMemory);
    DepthImageView = CreateImageView(DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    TransitionImageLayout(DepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void VulkanSwapchainManager::CreateFramebuffers()
{
    Framebuffers.resize(ImageViews.size());

    for (size_t i = 0; i < ImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = { ImageViews[i], DepthImageView };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = RenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = Extent->width;
        framebufferInfo.height = Extent->height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(Manager->GetLogicalDevice(), &framebufferInfo, nullptr, &Framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

void VulkanSwapchainManager::CreateUniformBuffers()
{
    VkDeviceSize cameraBufferSize = sizeof(UniformCameraObject);
    VkDeviceSize matrixBufferSize = sizeof(FMatrix4);
    VkDeviceSize intBufferSize = sizeof(int);

    CameraData.resize(Images.size());
    LightsData.resize(Images.size());
    NumberOfLightsData.resize(Images.size());


    for (size_t i = 0; i < Images.size(); i++)
    {
        Manager->CreateBuffer(cameraBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, CameraData[i].Buffer, CameraData[i].Memory);
        if (!Manager->GetRenderData()->LightSources.empty())
        {
            Manager->CreateBuffer(matrixBufferSize * Manager->GetRenderData()->LightSources.size(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, LightsData[i].Buffer, LightsData[i].Memory);
            Manager->CreateBuffer(intBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, NumberOfLightsData[i].Buffer, NumberOfLightsData[i].Memory);
        }

        for (const auto& material : Manager->GetRenderData()->Materials)
        {
            MaterialMap[&material->Data].resize(Images.size());
            Manager->CreateBuffer(matrixBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, MaterialMap[&material->Data][i].Buffer, MaterialMap[&material->Data][i].Memory);
        }

        for (const auto& model : Manager->GetRenderData()->Models)
        {
            ModelMap[model.first].resize(Images.size());
            Manager->CreateBuffer(matrixBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ModelMap[model.first][i].Buffer, ModelMap[model.first][i].Memory);
        }
    }
}

void VulkanSwapchainManager::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding cameraLayoutBinding{};
    cameraLayoutBinding.binding = 0;
    cameraLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    cameraLayoutBinding.descriptorCount = 1;
    cameraLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    cameraLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutBinding modelLayoutBinding{};
    modelLayoutBinding.binding = 1;
    modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    modelLayoutBinding.descriptorCount = 1;
    modelLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    modelLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 2;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding numberOfLightsLayoutBinding{};
    numberOfLightsLayoutBinding.binding = 3;
    numberOfLightsLayoutBinding.descriptorCount = 1;
    numberOfLightsLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    numberOfLightsLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    numberOfLightsLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding lightsLayoutBinding{};
    lightsLayoutBinding.binding = 4;
    lightsLayoutBinding.descriptorCount = 1;
    lightsLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lightsLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    lightsLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding materialLayoutBinding{};
    materialLayoutBinding.binding = 5;
    materialLayoutBinding.descriptorCount = 1;
    materialLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    materialLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    materialLayoutBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 6> bindings = { cameraLayoutBinding, modelLayoutBinding, samplerLayoutBinding, lightsLayoutBinding, numberOfLightsLayoutBinding, materialLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(Manager->GetLogicalDevice(), &layoutInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void VulkanSwapchainManager::CreateDescriptorPool()
{
    if (Manager->GetRenderData()->Models.empty())
    {
        DebugLogger::Warning("Nothing to be rendered, no descriptor pool created", "Renderers/Vulkan/VulkanSwapchainManager.cppp", __LINE__);
        return;
    }
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(Images.size() * Manager->GetRenderData()->Models.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(Images.size() * Manager->GetRenderData()->Models.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(Images.size() * Manager->GetRenderData()->Models.size());
    //if (poolInfo.maxSets < 1) poolInfo.maxSets = 1;

    if (vkCreateDescriptorPool(Manager->GetLogicalDevice(), &poolInfo, nullptr, &DescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void VulkanSwapchainManager::CreateDescriptorSets()
{
    if (Manager->GetRenderData()->Models.empty())
    {
        DebugLogger::Warning("Nothing to be rendered, no descriptor set created", "Renderers/Vulkan/VulkanSwapchainManager.cppp", __LINE__);
        return;
    }

    for (const auto& model : Manager->GetRenderData()->Models)
    {
        std::vector<VkDescriptorSetLayout> layouts(Images.size(), DescriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = DescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(Images.size());
        allocInfo.pSetLayouts = layouts.data();

        DescriptorSetsMap[model.first].resize(Images.size());
        if (vkAllocateDescriptorSets(Manager->GetLogicalDevice(), &allocInfo, DescriptorSetsMap[model.first].data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
    }

    for (size_t i = 0; i < Images.size(); i++)
    {
        VkDescriptorBufferInfo cameraInfo{};
        cameraInfo.buffer = CameraData[i].Buffer;
        cameraInfo.offset = 0;
        cameraInfo.range = sizeof(UniformCameraObject);

        VkDescriptorBufferInfo lightsInfo{};
        lightsInfo.buffer = LightsData[i].Buffer;
        lightsInfo.offset = 0;
        lightsInfo.range = sizeof(FMatrix4) * Manager->GetRenderData()->LightSources.size();

        VkDescriptorBufferInfo numberOfLightsInfo{};
        numberOfLightsInfo.buffer = NumberOfLightsData[i].Buffer;
        numberOfLightsInfo.offset = 0;
        numberOfLightsInfo.range = sizeof(int);

        for (const auto& material : Manager->GetRenderData()->MeshesByMaterial)
        {
            VkDescriptorBufferInfo materialInfo{};
            materialInfo.buffer = MaterialMap[&material.first->Data][i].Buffer;
            materialInfo.offset = 0;
            materialInfo.range = sizeof(FMatrix4);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = TextureDataMap[material.first->TextureDifuse].TextureImageView;
            imageInfo.sampler = TextureDataMap[material.first->TextureDifuse].TextureSampler;

            
            for (const auto& mesh : material.second) for (const auto& model : Manager->GetRenderData()->InstancesByMesh[mesh])
            {
                VkDescriptorBufferInfo modelInfo{};
                modelInfo.buffer = ModelMap[model][i].Buffer;
                modelInfo.offset = 0;
                modelInfo.range = sizeof(FMatrix4);

                std::array<VkWriteDescriptorSet, 6> descriptorWrites{};

                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = DescriptorSetsMap[model][i];
                descriptorWrites[0].dstBinding = 0;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &cameraInfo;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = DescriptorSetsMap[model][i];
                descriptorWrites[1].dstBinding = 1;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pBufferInfo = &modelInfo;

                descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[2].dstSet = DescriptorSetsMap[model][i];
                descriptorWrites[2].dstBinding = 2;
                descriptorWrites[2].dstArrayElement = 0;
                descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[2].descriptorCount = 1;
                descriptorWrites[2].pImageInfo = &imageInfo;

                descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[3].dstSet = DescriptorSetsMap[model][i];
                descriptorWrites[3].dstBinding = 3;
                descriptorWrites[3].dstArrayElement = 0;
                descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[3].descriptorCount = 1;
                descriptorWrites[3].pBufferInfo = &numberOfLightsInfo;

                descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[4].dstSet = DescriptorSetsMap[model][i];
                descriptorWrites[4].dstBinding = 4;
                descriptorWrites[4].dstArrayElement = 0;
                descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[4].descriptorCount = 1;
                descriptorWrites[4].pBufferInfo = &lightsInfo;

                descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[5].dstSet = DescriptorSetsMap[model][i];
                descriptorWrites[5].dstBinding = 5;
                descriptorWrites[5].dstArrayElement = 0;
                descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[5].descriptorCount = 1;
                descriptorWrites[5].pBufferInfo = &materialInfo;

                vkUpdateDescriptorSets(Manager->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }
        }
    }
}

void VulkanSwapchainManager::CreateTextureImage()//TODO remove depricated code
{
    for (const auto& material : Manager->GetRenderData()->Materials)
    {
        TextureLoader::LoadTexture(material->TextureDifuse->Path, material->TextureDifuse);
        
        int textureWidth = material->TextureDifuse->Width;
        int textureHeight = material->TextureDifuse->Height;
        unsigned char* texturePixels = static_cast<unsigned char*>(material->TextureDifuse->Pixels);
        //void* texturePixels = texture.first->TextureDifuse->Pixels;//TODO this may break Vulakn!!!


        VkDeviceSize imageSize = textureWidth * textureHeight * 4;

        if (!texturePixels)
        {
            throw std::runtime_error("Failed to load texture image!");
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        Manager->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        void* data;
        vkMapMemory(Manager->GetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, texturePixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(Manager->GetLogicalDevice(), stagingBufferMemory);

        stbi_image_free(texturePixels);
        CreateImage(textureWidth, textureHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, TextureDataMap[material->TextureDifuse].TextureImage, TextureDataMap[material->TextureDifuse].TextureImageMemory);

        TransitionImageLayout(TextureDataMap[material->TextureDifuse].TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        CopyBufferToImage(stagingBuffer, TextureDataMap[material->TextureDifuse].TextureImage, static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight));
        TransitionImageLayout(TextureDataMap[material->TextureDifuse].TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(Manager->GetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(Manager->GetLogicalDevice(), stagingBufferMemory, nullptr);

        TextureDataMap[material->TextureDifuse].TextureImageView = CreateImageView(TextureDataMap[material->TextureDifuse].TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        CreateTextureSampler(TextureDataMap[material->TextureDifuse].TextureSampler);
    }
}

void VulkanSwapchainManager::CreateTextureSampler(VkSampler_T*& sampler)
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(Manager->GetLogicalDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create texture sampler!");
    }
}

void VulkanSwapchainManager::RecreationCleanUp()
{
    vkDestroyImageView(Manager->GetLogicalDevice(), DepthImageView, nullptr);
    vkDestroyImage(Manager->GetLogicalDevice(), DepthImage, nullptr);
    vkFreeMemory(Manager->GetLogicalDevice(), DepthImageMemory, nullptr);
    for (const auto& framebuffer : Framebuffers) vkDestroyFramebuffer(Manager->GetLogicalDevice(), framebuffer, nullptr);
    vkDestroyRenderPass(Manager->GetLogicalDevice(), RenderPass, nullptr);
    for (size_t i = 0; i < ImageViews.size(); i++) vkDestroyImageView(Manager->GetLogicalDevice(), ImageViews[i], nullptr);
    vkDestroySwapchainKHR(Manager->GetLogicalDevice(), Swapchain, nullptr);
    for (const auto& data : CameraData)
    { 
        vkDestroyBuffer(Manager->GetLogicalDevice(), data.Buffer, nullptr);
        vkFreeMemory(Manager->GetLogicalDevice(), data.Memory, nullptr);
    }
    for (const auto& model : ModelMap)
    {
        for (const auto& modelData : model.second) 
        { 
            vkDestroyBuffer(Manager->GetLogicalDevice(), modelData.Buffer, nullptr);
            vkFreeMemory(Manager->GetLogicalDevice(), modelData.Memory, nullptr);
        }
    }
    vkDestroyDescriptorPool(Manager->GetLogicalDevice(), DescriptorPool, nullptr);
}

void VulkanSwapchainManager::FinalCleanUp()
{
    RecreationCleanUp();
    for (const auto& textureData : TextureDataMap)
    {
        vkDestroySampler(Manager->GetLogicalDevice(), textureData.second.TextureSampler, nullptr);
        vkDestroyImageView(Manager->GetLogicalDevice(), textureData.second.TextureImageView, nullptr);
        vkDestroyImage(Manager->GetLogicalDevice(), textureData.second.TextureImage, nullptr);
        vkFreeMemory(Manager->GetLogicalDevice(), textureData.second.TextureImageMemory, nullptr);
    }
    vkDestroyDescriptorSetLayout(Manager->GetLogicalDevice(), DescriptorSetLayout, nullptr);
}

VkFormat VulkanSwapchainManager::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(Manager->GetPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported format!");
}

VkFormat VulkanSwapchainManager::FindDepthFormat()
{
    return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

}

void VulkanSwapchainManager::TransitionImageLayout(VkImage_T* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = Manager->BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (HasStencilComponent(format))
        {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    Manager->EndSingleTimeCommands(commandBuffer);
}

bool VulkanSwapchainManager::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void VulkanSwapchainManager::CopyBufferToImage(VkBuffer_T* buffer, VkImage_T* image, unsigned int width, unsigned int height)
{
    VkCommandBuffer commandBuffer = Manager->BeginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    Manager->EndSingleTimeCommands(commandBuffer);
}

void VulkanSwapchainManager::UpdateBuffers(unsigned int currentImageIndex)
{
    auto vulkanCamera = *Manager->GetRenderData()->Camera;
    vulkanCamera.Projection[1][1] *= -1;
    void* cameraData;
    vkMapMemory(Manager->GetLogicalDevice(), CameraData[currentImageIndex].Memory, 0, sizeof(UniformCameraObject), 0, &cameraData);
    memcpy(cameraData, &vulkanCamera, sizeof(UniformCameraObject));
    vkUnmapMemory(Manager->GetLogicalDevice(), CameraData[currentImageIndex].Memory);

    void* numberOfLightsData;
    vkMapMemory(Manager->GetLogicalDevice(), NumberOfLightsData[currentImageIndex].Memory, 0, sizeof(int), 0, &numberOfLightsData);
    int numberOfLights = Manager->GetRenderData()->LightSources.size();
    memcpy(numberOfLightsData, &numberOfLights, sizeof(int));
    vkUnmapMemory(Manager->GetLogicalDevice(), NumberOfLightsData[currentImageIndex].Memory);

    void* lightsData;
    vkMapMemory(Manager->GetLogicalDevice(), LightsData[currentImageIndex].Memory, 0, sizeof(FMatrix4) * numberOfLights, 0, &lightsData);
    memcpy(lightsData, *Manager->GetRenderData()->LightSources.data(), sizeof(FMatrix4) * numberOfLights);
    vkUnmapMemory(Manager->GetLogicalDevice(), LightsData[currentImageIndex].Memory);

    for (const auto& material : Manager->GetRenderData()->Materials)
    {
        void* materialData;
        vkMapMemory(Manager->GetLogicalDevice(), MaterialMap[&material->Data][currentImageIndex].Memory, 0, sizeof(FMatrix4), 0, &materialData);
        memcpy(materialData, material, sizeof(FMatrix4));
        vkUnmapMemory(Manager->GetLogicalDevice(), MaterialMap[&material->Data][currentImageIndex].Memory);
    }

    for (const auto& model : Manager->GetRenderData()->Models)
    {
        void* modelData;
        vkMapMemory(Manager->GetLogicalDevice(), ModelMap[model.first][currentImageIndex].Memory, 0, sizeof(FMatrix4), 0, &modelData);
        memcpy(modelData, model.first, sizeof(FMatrix4));
        vkUnmapMemory(Manager->GetLogicalDevice(), ModelMap[model.first][currentImageIndex].Memory);
    }
}
