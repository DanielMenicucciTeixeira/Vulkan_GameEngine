#ifndef RENDERINITIALIZATIONDATA_H
#define RENDERINITIALIZATIONDATA_H

#include <unordered_map>
#include <vector>
#include <set>
#include "RenderObject.h"
#include "GameObject.h"
#include "StaticMeshComponent.h"

struct RenderInitializationData
{
	std::unordered_map<S_Mesh*, std::set<S_Texture*>> MeshToMaterialMap;
	std::unordered_map<S_Texture*, std::vector<FMatrix4*>> MaterialToModelMap;
	std::vector<FMatrix4*> Models;
	UniformCameraObject* Camera;

	void LoadGameObject(O_GameObject* gameObject)
	{
		for (const auto& staticMesh : gameObject->GetComponentsOfClass<C_StaticMeshComponent>())
		{
			MeshToMaterialMap[staticMesh->GetMesh()].insert(staticMesh->GetTexture());
			MaterialToModelMap[staticMesh->GetTexture()].push_back(staticMesh->GetModelMatrix());
			Models.push_back(staticMesh->GetModelMatrix());
		}
	}
};
#endif // !RENDERINITIALIZATIONDATA_H

