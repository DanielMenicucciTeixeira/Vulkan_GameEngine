#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "UniformBufferObject.h"
#include "Vertex.h"
#include <vector>
#include <array>
#include "Math\FVector3.h"
#include "Math\FVector2.h"
#include "Math\FVector4.h"
#include "Math\FMatrix4.h"
#include <string>
#include <SDL_image.h>
#include <memory>

struct S_Texture
{
public:
	unsigned short TextureID = 0;
	int Width = 0;
	int Height = 0;
	int Channels = 0;
	uint8_t BytesPerPixel = 0;
	void* Pixels = nullptr;
	//size_t ImageSize = 0; TODO implement proper sizing, at the moment every image is assumed to have the same byte/pixel ratio.

	std::string Path = "";
	std::string Name = "";
};

struct S_CubeSampler
{
public:
	S_CubeSampler()
	{
		for (int i = 0; i < 6; i++) Textures[i] = new S_Texture();
	}
	S_CubeSampler(S_Texture* textures[6])
	{ 
		for (int i = 0; i < 6; i++) Textures[i] = textures[i];
	}

	virtual ~S_CubeSampler()
	{
		for (int i = 0; i < 6; i++) if (Textures[i]) delete(Textures[i]);
	}

	std::string Name = "";

	S_Texture* Textures[6];

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

	//[#]X--------------Y---------------Z-----------W
	//[0]Ambient.X------Ambient.Y-------Ambient.Z---0
	//[1]Diffuse.X------Diffuse.Y-------Diffuse.Z---0
	//[2]Specular.X-----Specular.Y------Specular.Z--0
	//[3]Shinines-------Opacity---------0-----------0
	FMatrix4 Data = FMatrix4
	{
		1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, 1.0f, 0.0f,
		32.0f, 1.0f, 0.0f, 0.0f
	};

	S_Texture* TextureDifuse = nullptr;
	std::string TextureNameDifuse = "";
	
	S_Texture* TextureSpecular = nullptr;
	std::string TextureNameSpecular = "";

	std::string ShaderName;
	std::string Name;
};

struct S_Rectangle
{
	std::array<S_Vertex, 4> Vertices;

	std::array<unsigned int, 6> Indices;

	virtual ~S_Rectangle() {}
	
	S_Rectangle()
	{
		Vertices =
		{
		S_Vertex({-0.5f, -0.5f, 0.0f}, {0.f, 0.f, 0.0f}, {0.0f, 0.0f, 1.0f}),
		S_Vertex({0.5f, -0.5f, 0.0f}, {1.0f, 0.f, 0.0f}, {0.0f, 0.0f, 1.0f}),
		S_Vertex({0.5f, 0.5f, 0.0f}, {1.f, 1.f, 0.0f}, {0.0f, 0.0f, 1.0f}),
		S_Vertex({-0.5f, 0.5f, 0.0f}, {0.f, 1.f, 0.0f}, {0.0f, 0.0f, 1.0f})
		};

		Indices = { 0, 1, 2, 2, 3, 0 };
	};


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
