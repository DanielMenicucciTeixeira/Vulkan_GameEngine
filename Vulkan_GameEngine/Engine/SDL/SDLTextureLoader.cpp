#include "SDLTextureLoader.h"
#include <SDL.h>
#include <iostream>

SDLTextureLoader::SDLTextureLoader()
{
}

SDLTextureLoader::~SDLTextureLoader()
{
}

bool SDLTextureLoader::LoadTexture(const char* filePath, SDL_Surface** surface)
{
    //Load splash image
    *surface = SDL_LoadBMP(filePath);//TODO implement extension query and support for more extensions
    if (*surface == nullptr)//If no valid surface is provided, or image is not found return error.
    {
        printf("Unable to load image %s! SDL Error: %s\n", filePath, SDL_GetError());
        return false;
    }

    return true;
}
