#include "SDLTextureHandler.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

std::unique_ptr<SDLTextureHandler> SDLTextureHandler::Instance = nullptr;
std::vector<S_Texture*> SDLTextureHandler::Textures = std::vector<S_Texture*>();

SDLTextureHandler::SDLTextureHandler()
{
    Textures.reserve(10);
}

SDLTextureHandler* SDLTextureHandler::GetInstance()
{
    if (Instance.get() == nullptr) Instance.reset(new SDLTextureHandler);
    return Instance.get();
}

SDLTextureHandler::~SDLTextureHandler()
{
    CleanUp();
}

bool SDLTextureHandler::LoadTexture(const std::string& textureName, const std::string& filePath, S_Texture* outTexture)
{
    if (outTexture) DebugLogger::Warning(textureName + "was not nullptr, it will be overriden", "SDL/SDLTextureHandler.cpp", __LINE__);
    else outTexture = new S_Texture();
    SDL_Surface* surface = nullptr;
    surface = IMG_Load(filePath.c_str());
    if (surface == nullptr)
    {
        DebugLogger::Error("Failed to create surface for texture: " + textureName + " at: " + filePath + ".\nSDL_image Error: " + std::string(IMG_GetError()) + "\n", "SDL/SDLTextureHandler.cpp", __LINE__);
        return false;
    }

    outTexture->Height = surface->h;
    outTexture->Width = surface->w;
    outTexture->BytesPerPixel = surface->format->BytesPerPixel;
    outTexture->Pixels = surface->pixels;
    outTexture->Name = textureName;
    outTexture->Path = filePath;

    //SDL_FreeSurface(surface);
    return true;
}

void SDLTextureHandler::CleanUp()
{
}

const S_Texture* SDLTextureHandler::GetTextureData(const std::string& textureName) const
{
    for (const auto texture : Textures) if (texture->Name == textureName) return texture;
    DebugLogger::Warning("Texture not found! Texture name: " + textureName, "SDL/SDLTextureHandler.cpp", __LINE__);
    return nullptr;
}
