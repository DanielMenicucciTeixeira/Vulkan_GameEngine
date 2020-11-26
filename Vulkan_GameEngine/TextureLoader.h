#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>

struct TextureStruct;

class TextureLoader
{
public:
	static void LoadTexture(std::string texturePath, TextureStruct* texture);
};
#endif

