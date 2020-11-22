#include "VulkanSwapchain.h"
#include "VulkanManager.h"
#include "SDL/VGE_SDLManager.h"
#include "UniformCameraObject.h"

#include <vulkan/vulkan.h>
#include <SDL_vulkan.h>
#include<SDL.h>
#include<cmath>
#include <algorithm>
#include<fstream>
#include<array>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VulkanSwapchain::VulkanSwapchain(VulkanManager* manager)
{
	Manager = manager;
}

VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats)
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

VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities)
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

void VulkanSwapchain::CreateSwapChain()
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

    if (vkCreateSwapchainKHR(Manager->GetLogicalDevice(), &createInfo, nullptr, &SwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create SwapChain!");
    }

    vkGetSwapchainImagesKHR(Manager->GetLogicalDevice(), SwapChain, &imageCount, nullptr);
    SwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(Manager->GetLogicalDevice(), SwapChain, &imageCount, SwapChainImages.data());

    SwapChainImageFormat = surfaceFormat.format;
    SwapChainExtent = new VkExtent2D(extent);
}

void VulkanSwapchain::RecreateSwapChain()
{
    int width = 0;
    int height = 0;

    do
    {
        SDL_GetWindowSize(Manager->GetWindow(), &width, &height);
        SDL_WaitEvent(&Manager->GetWindowManager()->GetEvent());
    } while (width == 0 || height == 0);

    vkDeviceWaitIdle(Manager->GetLogicalDevice());
    CleanUpSwapChain();

    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    //CreateGraphicsPipeline();
    CreateDepthResources();
    CreateFramebuffers();
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();
    //CreateCommandBuffers();
}

void VulkanSwapchain::CreateImageViews()
{
    SwapChainImageViews.resize(SwapChainImages.size());

    for (uint32_t i = 0; i < SwapChainImages.size(); i++) {
        SwapChainImageViews[i] = CreateImageView(SwapChainImages[i], SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

VkImageView_T* VulkanSwapchain::CreateImageView(VkImage_T* image, VkFormat format, VkImageAspectFlags aspectFlags)
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

void VulkanSwapchain::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = SwapChainImageFormat;
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

void VulkanSwapchain::CreateImage(unsigned int width, unsigned int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage_T*& image, VkDeviceMemory_T*& imageMemory)
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

void VulkanSwapchain::CreateDepthResources()
{
    VkFormat depthFormat = FindDepthFormat();

    CreateImage(SwapChainExtent->width, SwapChainExtent->height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, DepthImage, DepthImageMemory);
    DepthImageView = CreateImageView(DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    TransitionImageLayout(DepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void VulkanSwapchain::CreateFramebuffers()
{
    SwapChainFramebuffers.resize(SwapChainImageViews.size());

    for (size_t i = 0; i < SwapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = { SwapChainImageViews[i], DepthImageView };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = RenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = SwapChainExtent->width;
        framebufferInfo.height = SwapChainExtent->height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(Manager->GetLogicalDevice(), &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

void VulkanSwapchain::CreateUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    UniformBuffers.resize(SwapChainImages.size());
    UniformBuffersMemory.resize(SwapChainImages.size());

    for (size_t i = 0; i < SwapChainImages.size(); i++)
    {
        Manager->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, UniformBuffers[i], UniformBuffersMemory[i]);
    }
}

void VulkanSwapchain::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(Manager->GetLogicalDevice(), &layoutInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void VulkanSwapchain::CreateDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(SwapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(SwapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(SwapChainImages.size());

    if (vkCreateDescriptorPool(Manager->GetLogicalDevice(), &poolInfo, nullptr, &DescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void VulkanSwapchain::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(SwapChainImages.size(), DescriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = DescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    DescriptorSets.resize(SwapChainImages.size());
    if (vkAllocateDescriptorSets(Manager->GetLogicalDevice(), &allocInfo, DescriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < SwapChainImages.size(); i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = UniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = TextureImageView;
        imageInfo.sampler = TextureSampler;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = DescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = DescriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;


        vkUpdateDescriptorSets(Manager->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void VulkanSwapchain::CreateTextureImage()
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        throw std::runtime_error("Failed to load texture image!");
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Manager->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void* data;
    vkMapMemory(Manager->GetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(Manager->GetLogicalDevice(), stagingBufferMemory);

    stbi_image_free(pixels);

    CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, TextureImage, TextureImageMemory);

    TransitionImageLayout(TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer, TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    TransitionImageLayout(TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(Manager->GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(Manager->GetLogicalDevice(), stagingBufferMemory, nullptr);
}

void VulkanSwapchain::CreateTextureImageView()
{
}

void VulkanSwapchain::CreateTextureSampler()
{
}

void VulkanSwapchain::CleanUpSwapChain()
{
    vkDestroyImageView(Manager->GetLogicalDevice(), DepthImageView, nullptr);
    vkDestroyImage(Manager->GetLogicalDevice(), DepthImage, nullptr);
    vkFreeMemory(Manager->GetLogicalDevice(), DepthImageMemory, nullptr);
    for (const auto& framebuffer : SwapChainFramebuffers) vkDestroyFramebuffer(Manager->GetLogicalDevice(), framebuffer, nullptr);
    //vkFreeCommandBuffers(Manager->GetLogicalDevice(), CommandPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());
    //vkDestroyPipeline(Manager->GetLogicalDevice(), GraphicsPipeline, nullptr);
    //vkDestroyPipelineLayout(Manager->GetLogicalDevice(), PipelineLayout, nullptr);
    vkDestroyRenderPass(Manager->GetLogicalDevice(), RenderPass, nullptr);
    for (size_t i = 0; i < SwapChainImageViews.size(); i++) vkDestroyImageView(Manager->GetLogicalDevice(), SwapChainImageViews[i], nullptr);
    vkDestroySwapchainKHR(Manager->GetLogicalDevice(), SwapChain, nullptr);
    for (const auto& buffer : UniformBuffers) { vkDestroyBuffer(Manager->GetLogicalDevice(), buffer, nullptr); }
    for (const auto& memory : UniformBuffersMemory) { vkFreeMemory(Manager->GetLogicalDevice(), memory, nullptr); }
    vkDestroyDescriptorPool(Manager->GetLogicalDevice(), DescriptorPool, nullptr);
}

VkFormat VulkanSwapchain::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
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

VkFormat VulkanSwapchain::FindDepthFormat()
{
    return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

}

void VulkanSwapchain::TransitionImageLayout(VkImage_T* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
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

bool VulkanSwapchain::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void VulkanSwapchain::CopyBufferToImage(VkBuffer_T* buffer, VkImage_T* image, unsigned int width, unsigned int height)
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
