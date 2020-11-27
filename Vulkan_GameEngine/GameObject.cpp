#include "GameObject.h"

GameObject::~GameObject()
{
	if (UBO) delete(UBO);
	if (Mesh) delete(Mesh);
	if (Texture) delete(Texture);
}
