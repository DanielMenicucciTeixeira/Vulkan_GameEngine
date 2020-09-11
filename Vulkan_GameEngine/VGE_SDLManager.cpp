#include "VGE_SDLManager.h"
#include "VGE_Window.h"

#include <SDL_vulkan.h>
#include <stdio.h>
#include <SDL.h>

VGE_SDLManager::VGE_SDLManager()
{
}

VGE_SDLManager::~VGE_SDLManager()
{
   if(WindowManager != nullptr) delete(WindowManager);
}

void VGE_SDLManager::Begin()
{
    WindowManager = new VGE_Window();

    if (InitializeSDL())//If SDL does not initilize, don't run any SDL dependent functions
    {
        if (WindowManager->CreateWindow("Vulkan Window"))//If the Window is not created, don't run anything that depends on the window
        {
            //Get window surface
            WindowManager->SetEngineWindowSurface(SDL_GetWindowSurface(WindowManager->GetEngineWindow()));

            //Fill the surface white
            SDL_FillRect(WindowManager->GetEngineWindowSurface(), NULL, SDL_MapRGB(WindowManager->GetEngineWindowSurface()->format, 0xFF, 0xFF, 0xFF));

            //Update the surface
            SDL_UpdateWindowSurface(WindowManager->GetEngineWindow());

            //Wait two seconds
            SDL_Delay(2000);
        }
    }

}

void VGE_SDLManager::End()
{
    //Destroy window
    SDL_DestroyWindow(WindowManager->GetEngineWindow());

    //Quit SDL subsystems
    SDL_Quit();
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
