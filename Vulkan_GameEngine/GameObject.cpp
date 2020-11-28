#include "GameObject.h"
#include "RenderObject.h"

GameObject::GameObject()
{
	Mesh = new MeshStruct();
	Texture = new TextureStruct();
	Model = new FMatrix4();
}

GameObject::~GameObject()
{
}
