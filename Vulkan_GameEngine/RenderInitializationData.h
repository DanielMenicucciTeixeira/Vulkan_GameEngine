#ifndef RENDERINITIALIZATIONDATA_H
#define RENDERINITIALIZATIONDATA_H

#include <unordered_map>
#include <vector>
#include <set>
#include "RenderObject.h"
#include "GameObject.h"

struct RenderInitializationData
{
	std::unordered_map<MeshStruct*, std::set<TextureStruct*>> MeshToMaterialMap;
	std::unordered_map<TextureStruct*, std::vector<FMatrix4*>> MaterialToModelMap;
	std::vector<FMatrix4*> Models;
	UniformCameraObject* Camera;

	void LoadGameObject(O_GameObject* gameObject)
	{
		Models.push_back(gameObject->GetModelMatrixPointer());
	}
};
#endif // !RENDERINITIALIZATIONDATA_H

