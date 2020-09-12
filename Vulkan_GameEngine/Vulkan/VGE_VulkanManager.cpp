#include "VGE_VulkanManager.h"
#include <vulkan\vulkan.h>
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
}

void VGE_VulkanManager::run()
{
    initVulkan();
    mainLoop();
    cleanup();
}

void VGE_VulkanManager::TestMe()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";
}

void VGE_VulkanManager::initVulkan()
{
}

void VGE_VulkanManager::mainLoop()
{
}

void VGE_VulkanManager::cleanup()
{
}
