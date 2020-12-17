#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "UniformBufferObject.h";
#include "Vertex.h"
#include <vector>
#include "Math\FVector3.h"

struct S_Texture
{
	int Width, Height, Channels;
	unsigned char* Pixels;

	std::string Path;
	std::string Name;
};

struct S_Mesh
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	std::string Path;
	std::string Name;
};

struct S_Material
{
	FVector3 Ambient;
	FVector3 Specular;
	FVector3 Difuse;
	S_Texture* TextureDifuse = new S_Texture();
	S_Texture* TextureSpecular = new S_Texture();

	std::string Name;

	~S_Material()
	{
		if (TextureDifuse) delete(TextureDifuse);
		if (TextureSpecular) delete(TextureSpecular);
	}
};

struct RenderObject
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	UniformBufferObject UBO;
	S_Texture Texture;
};

#endif
