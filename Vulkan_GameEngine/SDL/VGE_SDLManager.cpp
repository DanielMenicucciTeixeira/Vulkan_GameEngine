#include "VGE_SDLManager.h"
#include "VGE_Window.h"
#include "VGE_SDLTextureLoader.h"
#include"VGE_SDLEventHandler.h"

#include <SDL_vulkan.h>
#include <stdio.h>
#include <SDL.h>

VGE_SDLManager::VGE_SDLManager()
{
}

VGE_SDLManager::~VGE_SDLManager()
{
   if(WindowManager != nullptr) delete(WindowManager);
   if (TextureLoader != nullptr) delete(TextureLoader);
}

void VGE_SDLManager::Begin()
{
    WindowManager = new VGE_Window();
    TextureLoader = new VGE_SDLTextureLoader();
    EventHandler = new VGE_SDLEventHandler();

    if (InitializeSDL())//If SDL does not initilize, don't run any SDL dependent functions
    {
        if (WindowManager->CreateWindow("Vulkan Window", (unsigned)800, (unsigned)600))//If the Window is not created, don't run anything that depends on the window
        {
            //Get window surface
            WindowManager->SetEngineWindowSurface(SDL_GetWindowSurface(WindowManager->GetEngineWindow()));
        }
    }

}

void VGE_SDLManager::End()
{


    //Deallocate surface
    SDL_FreeSurface(imageSurface);

    //Destroy window
    SDL_DestroyWindow(WindowManager->GetEngineWindow());

    //Quit SDL subsystems
    SDL_Quit();
}

E_EngineEvent VGE_SDLManager::GetEvent()
{
    return EventHandler->HandleEvents();
}

void VGE_SDLManager::Render()
{
    //Load media
    if (!TextureLoader->LoadTexture("Assets/Textures/hello_world.bmp", &imageSurface))
    {
        printf("Failed to load media!\n");
    }
    else
    {
        //Apply the image
        SDL_BlitSurface(imageSurface, NULL, WindowManager->GetEngineWindowSurface(), NULL);
    }

    //Update the surface
    SDL_UpdateWindowSurface(WindowManager->GetEngineWindow());
}

bool VGE_SDLManager::GetVulkanExtensions(std::vector<const char*> &extensionNames)
{
    uint32_t extensionCount;
    if (SDL_Vulkan_GetInstanceExtensions(WindowManager->GetEngineWindow(), &extensionCount, nullptr) == SDL_FALSE) return false;
    extensionNames.resize(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(WindowManager->GetEngineWindow(), &extensionCount, extensionNames.data());
	return true;
}

bool VGE_SDLManager::InitializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else return true;
}
