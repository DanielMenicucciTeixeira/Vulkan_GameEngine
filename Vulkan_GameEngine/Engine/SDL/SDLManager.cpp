#include "SDLManager.h"
#include "SDLWindowManager.h"
#include "SDLTextureLoader.h"
#include"SDLEventHandler.h"
#include "Window.h"
#include "Renderer.h"

#include <SDL_vulkan.h>
#include <stdio.h>
#include <SDL.h>
#include<iostream>

SDLManager::SDLManager()
{
}

SDLManager::~SDLManager()
{
   if(WindowManager != nullptr) delete(WindowManager);
   if (TextureLoader != nullptr) delete(TextureLoader);
}

void SDLManager::Begin()
{
    WindowManager = new SDLWindowManager(this);
    EventHandler = new SDLEventHandler(WindowRenderer);

    if (!InitializeSDL())//If SDL does not initilize, don't run any SDL dependent functions
    {
        throw std::runtime_error("SDL did not Initilize!");
    }
    else
    {
        if (!WindowManager->CreateWindow(GetDefaultWindowName(), WindowRenderer->GetType(), (unsigned)800, (unsigned)600))//If the Window is not created, don't run anything that depends on the window
        {
            throw std::runtime_error("SDL Window could not be created!");
        }
    }

}

void SDLManager::End()
{


    //Deallocate surface
    SDL_FreeSurface(imageSurface);

    //Destroy window
    for (const auto window : WindowManager->GetWindows()) delete(window.second);

    //Quit SDL subsystems
    SDL_Quit();
}

SDL_Event SDLManager::GetEvent()
{
    return EventHandler->HandleEvents();
}

bool SDLManager::GetVulkanExtensions(std::vector<const char*> &extensionNames)
{
    for (const auto window : WindowManager->GetWindows())
    {
        uint32_t extensionCount;
        if (SDL_Vulkan_GetInstanceExtensions(window.second->GetSDLWindow(), &extensionCount, nullptr) == SDL_FALSE) return false;
        extensionNames.resize(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window.second->GetSDLWindow(), &extensionCount, extensionNames.data());
        return true;
    }
}

void SDLManager::SetRenderer(Renderer* renderer)
{
    WindowRenderer = renderer;
}

SDL_Window* SDLManager::GetSDLWindowByName(const char* name)
{
	return WindowManager->GetSDLWindowByName(name);
}

bool SDLManager::InitializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else return true;
}
