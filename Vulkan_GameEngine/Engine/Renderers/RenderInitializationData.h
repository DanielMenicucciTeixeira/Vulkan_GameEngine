#ifndef RENDERINITIALIZATIONDATA_H
#define RENDERINITIALIZATIONDATA_H

#include <unordered_map>
#include <vector>
#include <set>
#include "RenderObject.h"
#include "Objects/GameObjects/GameObject.h"
#include "Objects/Components/StaticMeshComponent.h"

struct RenderInitializationData
{
	std::unordered_map<S_Mesh*, std::set<S_Material*>> MeshToMaterialMap;
	std::unordered_map<S_Material*, std::vector<FMatrix4*>> MaterialToModelMap;
	std::vector<FMatrix4*> Models;
	UniformCameraObject* Camera;

	void LoadGameObject(O_GameObject* gameObject)
	{
		for (const auto& staticMesh : gameObject->GetComponentsOfClass<C_StaticMeshComponent>())
		{
			MeshToMaterialMap[staticMesh->GetMesh()].insert(staticMesh->GetMaterial());
			MaterialToModelMap[staticMesh->GetMaterial()].push_back(staticMesh->GetModelMatrix());
			Models.push_back(staticMesh->GetModelMatrix());
		}
	}

	~RenderInitializationData()
	{
	}
};
#endif // !RENDERINITIALIZATIONDATA_H

