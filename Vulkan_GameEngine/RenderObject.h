#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "UniformBufferObject.h";
#include "Vertex.h"
#include <vector>

struct TextureStruct
{
	int Width, Height, Channels;
	unsigned char* Pixels;
};

struct MeshStruct
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
};

struct RenderObject
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	UniformBufferObject UBO;
	TextureStruct Texture;
};

#endif
