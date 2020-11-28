#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct MeshStruct;
struct TextureStruct;
struct UniformBufferObject;
class FMatrix4;

class GameObject
{
public:
	GameObject();
	~GameObject();

	MeshStruct* Mesh;
	TextureStruct* Texture;
	FMatrix4* Model;
};
#endif

