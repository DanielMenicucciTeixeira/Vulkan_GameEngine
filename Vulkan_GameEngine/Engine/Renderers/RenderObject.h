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
	/*struct S_Data
	{
		float Opacity = 1.0f;
		float Shininess = 32.0f;
		alignas(FVector4) FVector4 Ambient = FVector4(1.0f);
		alignas(FVector4) FVector4 Diffuse = FVector4(1.0f);
		alignas(FVector4) FVector4 Specular = FVector4(1.0f);
	};*/
	

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

struct RenderObject
{
public:
	std::vector<S_Vertex> Vertices;
	std::vector<unsigned int> Indices;
	UniformBufferObject UBO;
	S_Texture Texture;
};

#endif
