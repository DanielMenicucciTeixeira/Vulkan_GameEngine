#include "SDLWindowManager.h"
#include "SDLManager.h"
#include "Window.h"

#include <SDL.h>
#include <stdio.h>

SDLWindowManager::SDLWindowManager(SDLManager* manager)
{
    Manager = manager;
}

SDLWindowManager::~SDLWindowManager()
{
   for( const auto window : WindowsByName) SDL_GL_DeleteContext(window.second);
}

Window* SDLWindowManager::CreateWindow(const char* name, ERendererType rendererType, float width, float height, float positionX, float positionY)
{
    //Set window size
    unsigned WIDTH, HEIGHT;
    if (width < 0) WIDTH = ScreenWidth;
    else WIDTH = width;
    if (height < 0) HEIGHT = ScreenHeight;
    else HEIGHT = height;

    //Set window position
    float POSITION_X, POSITION_Y;
    if (positionX < 0) POSITION_X = SDL_WINDOWPOS_UNDEFINED;
    else POSITION_X = positionX;
    if (positionY < 0) POSITION_Y = SDL_WINDOWPOS_UNDEFINED;
    else POSITION_Y = positionY;

    WindowsByName[name] = new Window(this);
    if (WindowsByName[name]->OnCreate(name, rendererType, WIDTH, HEIGHT, POSITION_X, POSITION_Y)) return WindowsByName[name];
    else return nullptr;

}

SDL_Window* SDLWindowManager::GetSDLWindowByName(const char* name)
{
    return WindowsByName[name]->GetSDLWindow();
}
