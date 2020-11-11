#include "VGE_SDLWindowManager.h"
#include <SDL.h>
#include <stdio.h>

VGE_SDLWindowManager::VGE_SDLWindowManager()
{
}

VGE_SDLWindowManager::~VGE_SDLWindowManager()
{
    //if(EngineWindow != nullptr) delete(EngineWindow);
    //if(EngineWindowSurface != nullptr) delete(EngineWindowSurface);
    //TODO figure out why I can't delete "EngineWindow" and "EngineWindowSurface" when I end the application.
    SDL_GL_DeleteContext(EngineWindow);
}

bool VGE_SDLWindowManager::CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY)
{
    //Set window size
    unsigned WINDOW_SIZE_X, WINDOW_SIZE_Y;
    if (windowSizeX < 0) WINDOW_SIZE_X = ScreenWidth;
    else WINDOW_SIZE_X = windowSizeX;
    if (windowSizeY < 0) WINDOW_SIZE_Y = ScreenHeight;
    else WINDOW_SIZE_Y = windowSizeY;

    //Set window position
    float WINDOW_POSITION_X, WINDOW_POSITION_Y;
    if (windowPositionX < 0) WINDOW_POSITION_X = SDL_WINDOWPOS_UNDEFINED;
    else WINDOW_POSITION_X = windowPositionX;
    if (windowPositionY < 0) WINDOW_POSITION_Y = SDL_WINDOWPOS_UNDEFINED;
    else WINDOW_POSITION_Y = windowPositionY;

    EngineWindow = SDL_CreateWindow(windowName, WINDOW_POSITION_X, WINDOW_POSITION_Y, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_VULKAN|SDL_WINDOW_RESIZABLE);
    SDL_GL_CreateContext(EngineWindow);
    if (EngineWindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else return true;
}

SDL_Window* VGE_SDLWindowManager::GetEngineWindow()
{
	return EngineWindow;
}
