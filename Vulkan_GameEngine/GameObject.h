#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct UniformBufferObject;
struct MeshStruct;
struct TextureStruct;

class GameObject
{
public:
	~GameObject();

	UniformBufferObject* UBO;
	MeshStruct* Mesh;
	TextureStruct* Texture;
};

#endif

