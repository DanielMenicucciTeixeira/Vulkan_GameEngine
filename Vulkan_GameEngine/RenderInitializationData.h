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

	void LoadGameObject(GameObject* gameObject)
	{
		MeshToMaterialMap[gameObject->Mesh].insert(gameObject->Texture);
		MaterialToModelMap[gameObject->Texture].push_back(gameObject->Model);
		Models.push_back(gameObject->Model);
	}
};
#endif // !RENDERINITIALIZATIONDATA_H

