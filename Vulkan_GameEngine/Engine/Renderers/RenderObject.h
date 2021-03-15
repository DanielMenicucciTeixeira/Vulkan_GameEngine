#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "UniformBufferObject.h"
#include "Vertex.h"
#include <vector>
#include "Math\FVector3.h"
#include <string>
#include <SDL_image.h>

struct S_Texture
{
public:
	unsigned short TextureID = 0;
	int Width = 0;
	int Height = 0;
	int Channels = 0;
	uint8_t BytesPerPixel = 0;
	void* Pixels = nullptr;

	std::string Path = "";
	std::string Name = "";
};

struct S_Mesh
{
public:
	unsigned short MeshID = 0;
	std::vector<S_Vertex> Vertices;
	std::vector<unsigned int> Indices;

	std::string Path;
	std::string Name;

	~S_Mesh() {}
};

struct S_Material
{
public:
	unsigned short MaterialID = 0;
	FVector3 Ambient;
	FVector3 Specular;
	FVector3 Difuse;
	S_Texture* TextureDifuse = nullptr;
	std::string TextureNameDifuse = "";
	std::string TextureNameSpecular = "";
	S_Texture* TextureSpecular = nullptr;

	std::string ShaderName;
	std::string Name;
};

struct RenderObject
{
public:
	std::vector<S_Vertex> Vertices;
	std::vector<unsigned int> Indices;
	UniformBufferObject UBO;
	S_Texture Texture;
};

#endif
