#include "VGE_VulkanWindowManager.h"

#include<vulkan/vulkan.h>
#include<SDL_vulkan.h>
#include<SDL.h>
#include <iostream>

VGE_VulkanWindowManager::VGE_VulkanWindowManager()
{
}

VGE_VulkanWindowManager::VGE_VulkanWindowManager(SDL_Window* window)
{
    Window = window;
}

VGE_VulkanWindowManager::~VGE_VulkanWindowManager()
{
}

void VGE_VulkanWindowManager::CreateSurface(VkInstance_T* instance)
{
    if (SDL_Vulkan_CreateSurface(Window, instance, &Surface) != SDL_TRUE)
    {
        throw std::runtime_error("failed to create window surface!");
    }

    int waintingLine = 0;
}

void VGE_VulkanWindowManager::CreateSurface(VkInstance_T* instance, SDL_Window* window)
{
    if (SDL_Vulkan_CreateSurface(window, instance, &Surface) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

SDL_Window* VGE_VulkanWindowManager::GetWindow()
{
    return Window;
}
