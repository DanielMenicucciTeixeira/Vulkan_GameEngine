#include "SDLManager.h"
#include "SDLWindowManager.h"
#include "SDLTextureLoader.h"
#include"SDLEventHandler.h"
#include "Window.h"
#include "Renderers/Renderer.h"

#include <SDL_vulkan.h>
#include <stdio.h>
#include <SDL.h>
#include<iostream>


std::unique_ptr<SDLManager> SDLManager::Instance = nullptr;

SDLManager::SDLManager()
{
}

SDLManager::~SDLManager()
{
}

SDLManager* SDLManager::GetInstance()
{
    if(Instance.get() == nullptr) Instance.reset(new SDLManager);
    return Instance.get();
}

bool SDLManager::Begin()
{
    WindowManager = new SDLWindowManager(this);
    EventHandler = new SDLEventHandler(WindowRenderer);

    if (!InitializeSDL())//If SDL does not initilize, don't run any SDL dependent functions
    {
       printf("SDL did not Initilize!");
       return false;
    }

    return true;
}

void SDLManager::End()
{
    //Deallocate surface
    SDL_FreeSurface(imageSurface);

    //Destroy window
    for (const auto window : WindowManager->GetWindows()) delete(window.second);

    //Quit SDL subsystems
    SDL_Quit();

    if (WindowManager != nullptr) delete(WindowManager);
    if (TextureLoader != nullptr) delete(TextureLoader);
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

Window* SDLManager::CreateWindow(const char* windowName, ERendererType rendererType, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY)
{
    return WindowManager->CreateWindow(windowName, rendererType, windowSizeX, windowSizeY);//If the Window is not created, don't run anything that depends on the window
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
