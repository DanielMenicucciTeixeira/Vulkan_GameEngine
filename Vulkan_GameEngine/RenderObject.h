#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "UniformBufferObject.h";
#include "Vertex.h"
#include <vector>
#include "Math\FVector3.h"
#include <string>

struct S_Texture
{
public:
	int Width, Height, Channels;
	unsigned char* Pixels;

	std::string Path;
	char* Name;
};

struct S_Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	std::string Path;
	std::string Name;
};

struct S_Material
{
public:
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
public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	UniformBufferObject UBO;
	S_Texture Texture;
};

#endif
