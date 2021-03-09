#include "TextureLoader.h"

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Renderers/RenderObject.h"

void TextureLoader::LoadTexture(std::string texturePath, S_Texture* texture)
{
   
    texture->Pixels = stbi_load(texturePath.c_str(), &texture->Width, &texture->Height, &texture->Channels, STBI_rgb_alpha);
}
