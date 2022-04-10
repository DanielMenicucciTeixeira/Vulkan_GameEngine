#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>

struct S_Texture;

class TextureLoader
{
public:
	static bool LoadTexture(std::string texturePath, S_Texture* texture);
};
#endif

