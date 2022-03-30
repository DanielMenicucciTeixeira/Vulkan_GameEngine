#include "TextureHandler.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

std::unique_ptr<TextureHandler> TextureHandler::Instance = nullptr;
std::vector<S_Texture*> TextureHandler::Textures = std::vector<S_Texture*>();

TextureHandler::TextureHandler()
{
    Textures.reserve(10);
}

TextureHandler* TextureHandler::GetInstance()
{
    if (Instance.get() == nullptr) Instance.reset(new TextureHandler);
    return Instance.get();
}

TextureHandler::~TextureHandler()
{
    CleanUp();
}

bool TextureHandler::LoadTexture(const std::string& textureName, const std::string& filePath, S_Texture* outTexture)
{
    if (outTexture) DebugLogger::Warning(textureName + "was not nullptr, it will be overriden", "SDL/TextureHandler.cpp", __LINE__);
    else outTexture = new S_Texture();
    SDL_Surface* surface = nullptr;
    surface = IMG_Load(filePath.c_str());
    if (surface == nullptr)
    {
        DebugLogger::Error("Failed to create surface for texture: " + textureName + " at: " + filePath + ".\nSDL_image Error: " + std::string(IMG_GetError()) + "\n", "SDL/TextureHandler.cpp", __LINE__);
        return false;
    }


    outTexture->Height = surface->h;
    outTexture->Width = surface->w;
    outTexture->BytesPerPixel = surface->format->BytesPerPixel;
    outTexture->Pixels = surface->pixels;
    outTexture->Name = textureName;
    outTexture->Path = filePath;

    std::cout << "Texture Format of " << outTexture->Name << ": " << SDL_GetPixelFormatName(surface->format->format) << std::endl;

    GetInstance()->Surfaces.insert(surface);
    return true;
}

void TextureHandler::CleanUp()
{
    for (auto& surface : Surfaces) SDL_FreeSurface(surface);
}

const S_Texture* TextureHandler::GetTextureData(const std::string& textureName) const
{
    for (const auto texture : Textures) if (texture->Name == textureName) return texture;
    DebugLogger::Warning("Texture not found! Texture name: " + textureName, "SDL/TextureHandler.cpp", __LINE__);
    return nullptr;
}
